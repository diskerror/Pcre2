PCRE2_EXP_DEFN int PCRE2_CALL_CONVENTION
pcre2_match(const pcre2_code *code, PCRE2_SPTR subject, PCRE2_SIZE length,
            PCRE2_SIZE start_offset, uint32_t options, pcre2_match_data *match_data,
            pcre2_match_context *mcontext)
{
	int rc;
	const uint8_t *start_bits = NULL;

	const pcre2_real_code *re = (const pcre2_real_code *)code;

	BOOL anchored;
	BOOL firstline;
	BOOL has_first_cu = FALSE;
	BOOL has_req_cu = FALSE;
	BOOL startline;
	BOOL utf;

	PCRE2_UCHAR first_cu = 0;
	PCRE2_UCHAR first_cu2 = 0;
	PCRE2_UCHAR req_cu = 0;
	PCRE2_UCHAR req_cu2 = 0;

	PCRE2_SPTR bumpalong_limit;
	PCRE2_SPTR end_subject;
	PCRE2_SPTR start_match = subject + start_offset;
	PCRE2_SPTR req_cu_ptr = start_match - 1;
	PCRE2_SPTR start_partial = NULL;
	PCRE2_SPTR match_partial = NULL;

	PCRE2_SIZE frame_size;

/* We need to have mb as a pointer to a match block, because the IS_NEWLINE
macro is used below, and it expects NLBLOCK to be defined as a pointer. */

	pcre2_callout_block cb;
	match_block actual_match_block;
	match_block *mb = &actual_match_block;

/* Allocate an initial vector of backtracking frames on the stack. If this
proves to be too small, it is replaced by a larger one on the heap. To get a
vector of the size required that is aligned for pointers, allocate it as a
vector of pointers. */

	PCRE2_SPTR stack_frames_vector[START_FRAMES_SIZE/sizeof(PCRE2_SPTR)];
	mb->stack_frames = (heapframe *)stack_frames_vector;

/* A length equal to PCRE2_ZERO_TERMINATED implies a zero-terminated
subject string. */

	if (length == PCRE2_ZERO_TERMINATED) length = PRIV(strlen)(subject);
	end_subject = subject + length;

/* Plausibility checks */

	if ((options & ~PUBLIC_MATCH_OPTIONS) != 0) return PCRE2_ERROR_BADOPTION;
	if (code == NULL || subject == NULL || match_data == NULL)
		return PCRE2_ERROR_NULL;
	if (start_offset > length) return PCRE2_ERROR_BADOFFSET;

/* Check that the first field in the block is the magic number. */

	if (re->magic_number != MAGIC_NUMBER) return PCRE2_ERROR_BADMAGIC;

/* Check the code unit width. */

	if ((re->flags & PCRE2_MODE_MASK) != PCRE2_CODE_UNIT_WIDTH/8)
		return PCRE2_ERROR_BADMODE;

/* PCRE2_NOTEMPTY and PCRE2_NOTEMPTY_ATSTART are match-time flags in the
options variable for this function. Users of PCRE2 who are not calling the
function directly would like to have a way of setting these flags, in the same
way that they can set pcre2_compile() flags like PCRE2_NO_AUTOPOSSESS with
constructions like (*NO_AUTOPOSSESS). To enable this, (*NOTEMPTY) and
(*NOTEMPTY_ATSTART) set bits in the pattern's "flag" function which we now
transfer to the options for this function. The bits are guaranteed to be
adjacent, but do not have the same values. This bit of Boolean trickery assumes
that the match-time bits are not more significant than the flag bits. If by
accident this is not the case, a compile-time division by zero error will
occur. */

#define FF (PCRE2_NOTEMPTY_SET|PCRE2_NE_ATST_SET)
#define OO (PCRE2_NOTEMPTY|PCRE2_NOTEMPTY_ATSTART)
	options |= (re->flags & FF) / ((FF & (~FF+1)) / (OO & (~OO+1)));
#undef FF
#undef OO

/* These two settings are used in the code for checking a UTF string that
follows immediately afterwards. Other values in the mb block are used only
during interpretive processing, not when the JIT support is in use, so they are
set up later. */

	utf = (re->overall_options & PCRE2_UTF) != 0;
	mb->partial = ((options & PCRE2_PARTIAL_HARD) != 0)? 2 :
	              ((options & PCRE2_PARTIAL_SOFT) != 0)? 1 : 0;

/* Partial matching and PCRE2_ENDANCHORED are currently not allowed at the same
time. */

	if (mb->partial != 0 &&
		((re->overall_options | options) & PCRE2_ENDANCHORED) != 0)
		return PCRE2_ERROR_BADOPTION;

/* Check a UTF string for validity if required. For 8-bit and 16-bit strings,
we must also check that a starting offset does not point into the middle of a
multiunit character. We check only the portion of the subject that is going to
be inspected during matching - from the offset minus the maximum back reference
to the given length. This saves time when a small part of a large subject is
being matched by the use of a starting offset. Note that the maximum lookbehind
is a number of characters, not code units. */

#ifdef SUPPORT_UNICODE
	if (utf && (options & PCRE2_NO_UTF_CHECK) == 0)
  {
  PCRE2_SPTR check_subject = start_match;  /* start_match includes offset */

  if (start_offset > 0)
    {
#if PCRE2_CODE_UNIT_WIDTH != 32
    unsigned int i;
    if (start_match < end_subject && NOT_FIRSTCU(*start_match))
      return PCRE2_ERROR_BADUTFOFFSET;
    for (i = re->max_lookbehind; i > 0 && check_subject > subject; i--)
      {
      check_subject--;
      while (check_subject > subject &&
#if PCRE2_CODE_UNIT_WIDTH == 8
      (*check_subject & 0xc0) == 0x80)
#else  /* 16-bit */
      (*check_subject & 0xfc00) == 0xdc00)
#endif /* PCRE2_CODE_UNIT_WIDTH == 8 */
        check_subject--;
      }
#else
    /* In the 32-bit library, one code unit equals one character. However,
    we cannot just subtract the lookbehind and then compare pointers, because
    a very large lookbehind could create an invalid pointer. */

    if (start_offset >= re->max_lookbehind)
      check_subject -= re->max_lookbehind;
    else
      check_subject = subject;
#endif  /* PCRE2_CODE_UNIT_WIDTH != 32 */
    }

  /* Validate the relevant portion of the subject. After an error, adjust the
  offset to be an absolute offset in the whole string. */

  match_data->rc = PRIV(valid_utf)(check_subject,
    length - (check_subject - subject), &(match_data->startchar));
  if (match_data->rc != 0)
    {
    match_data->startchar += check_subject - subject;
    return match_data->rc;
    }
  }
#endif  /* SUPPORT_UNICODE */

/* It is an error to set an offset limit without setting the flag at compile
time. */

	if (mcontext != NULL && mcontext->offset_limit != PCRE2_UNSET &&
		(re->overall_options & PCRE2_USE_OFFSET_LIMIT) == 0)
		return PCRE2_ERROR_BADOFFSETLIMIT;

/* If the pattern was successfully studied with JIT support, run the JIT
executable instead of the rest of this function. Most options must be set at
compile time for the JIT code to be usable. Fallback to the normal code path if
an unsupported option is set or if JIT returns BADOPTION (which means that the
selected normal or partial matching mode was not compiled). */

#ifdef SUPPORT_JIT
	if (re->executable_jit != NULL && (options & ~PUBLIC_JIT_MATCH_OPTIONS) == 0)
  {
  rc = pcre2_jit_match(code, subject, length, start_offset, options,
    match_data, mcontext);
  if (rc != PCRE2_ERROR_JIT_BADOPTION) return rc;
  }
#endif

/* Carry on with non-JIT matching. A NULL match context means "use a default
context", but we take the memory control functions from the pattern. */

	if (mcontext == NULL)
	{
		mcontext = (pcre2_match_context *)(&PRIV(default_match_context));
		mb->memctl = re->memctl;
	}
	else mb->memctl = mcontext->memctl;

	anchored = ((re->overall_options | options) & PCRE2_ANCHORED) != 0;
	firstline = (re->overall_options & PCRE2_FIRSTLINE) != 0;
	startline = (re->flags & PCRE2_STARTLINE) != 0;
	bumpalong_limit =  (mcontext->offset_limit == PCRE2_UNSET)?
	                   end_subject : subject + mcontext->offset_limit;

/* Initialize and set up the fixed fields in the callout block, with a pointer
in the match block. */

	mb->cb = &cb;
	cb.version = 2;
	cb.subject = subject;
	cb.subject_length = (PCRE2_SIZE)(end_subject - subject);
	cb.callout_flags = 0;

/* Fill in the remaining fields in the match block. */

	mb->callout = mcontext->callout;
	mb->callout_data = mcontext->callout_data;

	mb->start_subject = subject;
	mb->start_offset = start_offset;
	mb->end_subject = end_subject;
	mb->hasthen = (re->flags & PCRE2_HASTHEN) != 0;

	mb->moptions = options;                 /* Match options */
	mb->poptions = re->overall_options;     /* Pattern options */

	mb->ignore_skip_arg = 0;
	mb->mark = mb->nomatch_mark = NULL;     /* In case never set */
	mb->hitend = FALSE;

/* The name table is needed for finding all the numbers associated with a
given name, for condition testing. The code follows the name table. */

	mb->name_table = (PCRE2_UCHAR *)((uint8_t *)re + sizeof(pcre2_real_code));
	mb->name_count = re->name_count;
	mb->name_entry_size = re->name_entry_size;
	mb->start_code = mb->name_table + re->name_count * re->name_entry_size;

/* Process the \R and newline settings. */

	mb->bsr_convention = re->bsr_convention;
	mb->nltype = NLTYPE_FIXED;
	switch(re->newline_convention)
	{
		case PCRE2_NEWLINE_CR:
			mb->nllen = 1;
			mb->nl[0] = CHAR_CR;
			break;

		case PCRE2_NEWLINE_LF:
			mb->nllen = 1;
			mb->nl[0] = CHAR_NL;
			break;

		case PCRE2_NEWLINE_NUL:
			mb->nllen = 1;
			mb->nl[0] = CHAR_NUL;
			break;

		case PCRE2_NEWLINE_CRLF:
			mb->nllen = 2;
			mb->nl[0] = CHAR_CR;
			mb->nl[1] = CHAR_NL;
			break;

		case PCRE2_NEWLINE_ANY:
			mb->nltype = NLTYPE_ANY;
			break;

		case PCRE2_NEWLINE_ANYCRLF:
			mb->nltype = NLTYPE_ANYCRLF;
			break;

		default: return PCRE2_ERROR_INTERNAL;
	}

/* The backtracking frames have fixed data at the front, and a PCRE2_SIZE
vector at the end, whose size depends on the number of capturing parentheses in
the pattern. It is not used at all if there are no capturing parentheses.

  frame_size             is the total size of each frame
  mb->frame_vector_size  is the total usable size of the vector (rounded down
                           to a whole number of frames)

The last of these is changed within the match() function if the frame vector
has to be expanded. We therefore put it into the match block so that it is
correct when calling match() more than once for non-anchored patterns. */

	frame_size = offsetof(heapframe, ovector) +
		re->top_bracket * 2 * sizeof(PCRE2_SIZE);

/* Limits set in the pattern override the match context only if they are
smaller. */

	mb->heap_limit = (mcontext->heap_limit < re->limit_heap)?
	                 mcontext->heap_limit : re->limit_heap;

	mb->match_limit = (mcontext->match_limit < re->limit_match)?
	                  mcontext->match_limit : re->limit_match;

	mb->match_limit_depth = (mcontext->depth_limit < re->limit_depth)?
	                        mcontext->depth_limit : re->limit_depth;

/* If a pattern has very many capturing parentheses, the frame size may be very
large. Ensure that there are at least 10 available frames by getting an initial
vector on the heap if necessary, except when the heap limit prevents this. Get
fewer if possible. (The heap limit is in kilobytes.) */

	if (frame_size <= START_FRAMES_SIZE/10)
	{
		mb->match_frames = mb->stack_frames;   /* Initial frame vector on the stack */
		mb->frame_vector_size = ((START_FRAMES_SIZE/frame_size) * frame_size);
	}
	else
	{
		mb->frame_vector_size = frame_size * 10;
		if ((mb->frame_vector_size / 1024) > mb->heap_limit)
		{
			if (frame_size > mb->heap_limit * 1024) return PCRE2_ERROR_HEAPLIMIT;
			mb->frame_vector_size = ((mb->heap_limit * 1024)/frame_size) * frame_size;
		}
		mb->match_frames = mb->memctl.malloc(mb->frame_vector_size,
		                                     mb->memctl.memory_data);
		if (mb->match_frames == NULL) return PCRE2_ERROR_NOMEMORY;
	}

	mb->match_frames_top =
		(heapframe *)((char *)mb->match_frames + mb->frame_vector_size);

/* Write to the ovector within the first frame to mark every capture unset and
to avoid uninitialized memory read errors when it is copied to a new frame. */

	memset((char *)(mb->match_frames) + offsetof(heapframe, ovector), 0xff,
	       re->top_bracket * 2 * sizeof(PCRE2_SIZE));

/* Pointers to the individual character tables */

	mb->lcc = re->tables + lcc_offset;
	mb->fcc = re->tables + fcc_offset;
	mb->ctypes = re->tables + ctypes_offset;

/* Set up the first code unit to match, if available. If there's no first code
unit there may be a bitmap of possible first characters. */

	if ((re->flags & PCRE2_FIRSTSET) != 0)
	{
		has_first_cu = TRUE;
		first_cu = first_cu2 = (PCRE2_UCHAR)(re->first_codeunit);
		if ((re->flags & PCRE2_FIRSTCASELESS) != 0)
		{
			first_cu2 = TABLE_GET(first_cu, mb->fcc, first_cu);
#if defined SUPPORT_UNICODE && PCRE2_CODE_UNIT_WIDTH != 8
			if (utf && first_cu > 127) first_cu2 = UCD_OTHERCASE(first_cu);
#endif
		}
	}
	else
	if (!startline && (re->flags & PCRE2_FIRSTMAPSET) != 0)
		start_bits = re->start_bitmap;

/* There may also be a "last known required character" set. */

	if ((re->flags & PCRE2_LASTSET) != 0)
	{
		has_req_cu = TRUE;
		req_cu = req_cu2 = (PCRE2_UCHAR)(re->last_codeunit);
		if ((re->flags & PCRE2_LASTCASELESS) != 0)
		{
			req_cu2 = TABLE_GET(req_cu, mb->fcc, req_cu);
#if defined SUPPORT_UNICODE && PCRE2_CODE_UNIT_WIDTH != 8
			if (utf && req_cu > 127) req_cu2 = UCD_OTHERCASE(req_cu);
#endif
		}
	}


/* ==========================================================================*/

/* Loop for handling unanchored repeated matching attempts; for anchored regexs
the loop runs just once. */

	for(;;)
	{
		PCRE2_SPTR new_start_match;

		/* ----------------- Start of match optimizations ---------------- */

		/* There are some optimizations that avoid running the match if a known
		starting point is not found, or if a known later code unit is not present.
		However, there is an option (settable at compile time) that disables these,
		for testing and for ensuring that all callouts do actually occur. */

		if ((re->overall_options & PCRE2_NO_START_OPTIMIZE) == 0)
		{
			/* If firstline is TRUE, the start of the match is constrained to the first
			line of a multiline string. That is, the match must be before or at the
			first newline following the start of matching. Temporarily adjust
			end_subject so that we stop the scans for a first code unit at a newline.
			If the match fails at the newline, later code breaks the loop. */

			if (firstline)
			{
				PCRE2_SPTR t = start_match;
#ifdef SUPPORT_UNICODE
				if (utf)
        {
        while (t < end_subject && !IS_NEWLINE(t))
          {
          t++;
          ACROSSCHAR(t < end_subject, t, t++);
          }
        }
      else
#endif
				while (t < end_subject && !IS_NEWLINE(t)) t++;
				end_subject = t;
			}

			/* Anchored: check the first code unit if one is recorded. This may seem
			pointless but it can help in detecting a no match case without scanning for
			the required code unit. */

			if (anchored)
			{
				if (has_first_cu || start_bits != NULL)
				{
					BOOL ok = start_match < end_subject;
					if (ok)
					{
						PCRE2_UCHAR c = UCHAR21TEST(start_match);
						ok = has_first_cu && (c == first_cu || c == first_cu2);
						if (!ok && start_bits != NULL)
						{
#if PCRE2_CODE_UNIT_WIDTH != 8
							if (c > 255) c = 255;
#endif
							ok = (start_bits[c/8] & (1 << (c&7))) != 0;
						}
					}
					if (!ok)
					{
						rc = MATCH_NOMATCH;
						break;
					}
				}
			}

				/* Not anchored. Advance to a unique first code unit if there is one. In
				8-bit mode, the use of memchr() gives a big speed up, even though we have
				to call it twice in caseless mode, in order to find the earliest occurrence
				of the character in either of its cases. */

			else
			{
				if (has_first_cu)
				{
					if (first_cu != first_cu2)  /* Caseless */
					{
#if PCRE2_CODE_UNIT_WIDTH != 8
						PCRE2_UCHAR smc;
						while (start_match < end_subject &&
							(smc = UCHAR21TEST(start_match)) != first_cu &&
							smc != first_cu2)
							start_match++;
#else  /* 8-bit code units */
						PCRE2_SPTR pp1 =
            memchr(start_match, first_cu, end_subject-start_match);
          PCRE2_SPTR pp2 =
            memchr(start_match, first_cu2, end_subject-start_match);
          if (pp1 == NULL)
            start_match = (pp2 == NULL)? end_subject : pp2;
          else
            start_match = (pp2 == NULL || pp1 < pp2)? pp1 : pp2;
#endif
					}

						/* The caseful case */

					else
					{
#if PCRE2_CODE_UNIT_WIDTH != 8
						while (start_match < end_subject && UCHAR21TEST(start_match) !=
							first_cu)
							start_match++;
#else
						start_match = memchr(start_match, first_cu, end_subject - start_match);
          if (start_match == NULL) start_match = end_subject;
#endif
					}

					/* If we can't find the required code unit, having reached the true end
					of the subject, break the bumpalong loop, to force a match failure,
					except when doing partial matching, when we let the next cycle run at
					the end of the subject. To see why, consider the pattern /(?<=abc)def/,
					which partially matches "abc", even though the string does not contain
					the starting character "d". If we have not reached the true end of the
					subject (PCRE2_FIRSTLINE caused end_subject to be temporarily modified)
					we also let the cycle run, because the matching string is legitimately
					allowed to start with the first code unit of a newline. */

					if (!mb->partial && start_match >= mb->end_subject)
					{
						rc = MATCH_NOMATCH;
						break;
					}
				}

					/* If there's no first code unit, advance to just after a linebreak for a
					multiline match if required. */

				else if (startline)
				{
					if (start_match > mb->start_subject + start_offset)
					{
#ifdef SUPPORT_UNICODE
						if (utf)
            {
            while (start_match < end_subject && !WAS_NEWLINE(start_match))
              {
              start_match++;
              ACROSSCHAR(start_match < end_subject, start_match, start_match++);
              }
            }
          else
#endif
						while (start_match < end_subject && !WAS_NEWLINE(start_match))
							start_match++;

						/* If we have just passed a CR and the newline option is ANY or
						ANYCRLF, and we are now at a LF, advance the match position by one
						more code unit. */

						if (start_match[-1] == CHAR_CR &&
							(mb->nltype == NLTYPE_ANY || mb->nltype == NLTYPE_ANYCRLF) &&
							start_match < end_subject &&
							UCHAR21TEST(start_match) == CHAR_NL)
							start_match++;
					}
				}

					/* If there's no first code unit or a requirement for a multiline line
					start, advance to a non-unique first code unit if any have been
					identified. The bitmap contains only 256 bits. When code units are 16 or
					32 bits wide, all code units greater than 254 set the 255 bit. */

				else if (start_bits != NULL)
				{
					while (start_match < end_subject)
					{
						uint32_t c = UCHAR21TEST(start_match);
#if PCRE2_CODE_UNIT_WIDTH != 8
						if (c > 255) c = 255;
#endif
						if ((start_bits[c/8] & (1 << (c&7))) != 0) break;
						start_match++;
					}

					/* See comment above in first_cu checking about the next few lines. */

					if (!mb->partial && start_match >= mb->end_subject)
					{
						rc = MATCH_NOMATCH;
						break;
					}
				}
			}   /* End first code unit handling */

			/* Restore fudged end_subject */

			end_subject = mb->end_subject;

			/* The following two optimizations must be disabled for partial matching. */

			if (!mb->partial)
			{
				/* The minimum matching length is a lower bound; no string of that length
				may actually match the pattern. Although the value is, strictly, in
				characters, we treat it as code units to avoid spending too much time in
				this optimization. */

				if (end_subject - start_match < re->minlength)
				{
					rc = MATCH_NOMATCH;
					break;
				}

				/* If req_cu is set, we know that that code unit must appear in the
				subject for the (non-partial) match to succeed. If the first code unit is
				set, req_cu must be later in the subject; otherwise the test starts at
				the match point. This optimization can save a huge amount of backtracking
				in patterns with nested unlimited repeats that aren't going to match.
				Writing separate code for caseful/caseless versions makes it go faster,
				as does using an autoincrement and backing off on a match. As in the case
				of the first code unit, using memchr() in the 8-bit library gives a big
				speed up. Unlike the first_cu check above, we do not need to call
				memchr() twice in the caseless case because we only need to check for the
				presence of the character in either case, not find the first occurrence.

				HOWEVER: when the subject string is very, very long, searching to its end
				can take a long time, and give bad performance on quite ordinary
				patterns. This showed up when somebody was matching something like
				/^\d+C/ on a 32-megabyte string... so we don't do this when the string is
				sufficiently long. */

				if (has_req_cu && end_subject - start_match < REQ_CU_MAX)
				{
					PCRE2_SPTR p = start_match + (has_first_cu? 1:0);

					/* We don't need to repeat the search if we haven't yet reached the
					place we found it last time round the bumpalong loop. */

					if (p > req_cu_ptr)
					{
						if (p < end_subject)
						{
							if (req_cu != req_cu2)  /* Caseless */
							{
#if PCRE2_CODE_UNIT_WIDTH != 8
								do
								{
									uint32_t pp = UCHAR21INCTEST(p);
									if (pp == req_cu || pp == req_cu2) { p--; break; }
								}
								while (p < end_subject);

#else  /* 8-bit code units */
								PCRE2_SPTR pp = p;
              p = memchr(pp, req_cu, end_subject - pp);
              if (p == NULL)
                {
                p = memchr(pp, req_cu2, end_subject - pp);
                if (p == NULL) p = end_subject;
                }
#endif /* PCRE2_CODE_UNIT_WIDTH != 8 */
							}

								/* The caseful case */

							else
							{
#if PCRE2_CODE_UNIT_WIDTH != 8
								do
								{
									if (UCHAR21INCTEST(p) == req_cu) { p--; break; }
								}
								while (p < end_subject);

#else  /* 8-bit code units */
								p = memchr(p, req_cu, end_subject - p);
              if (p == NULL) p = end_subject;
#endif
							}
						}

						/* If we can't find the required code unit, break the bumpalong loop,
						forcing a match failure. */

						if (p >= end_subject)
						{
							rc = MATCH_NOMATCH;
							break;
						}

						/* If we have found the required code unit, save the point where we
						found it, so that we don't search again next time round the bumpalong
						loop if the start hasn't yet passed this code unit. */

						req_cu_ptr = p;
					}
				}
			}
		}

		/* ------------ End of start of match optimizations ------------ */

		/* Give no match if we have passed the bumpalong limit. */

		if (start_match > bumpalong_limit)
		{
			rc = MATCH_NOMATCH;
			break;
		}

		/* OK, we can now run the match. If "hitend" is set afterwards, remember the
		first starting point for which a partial match was found. */

		cb.start_match = (PCRE2_SIZE)(start_match - subject);
		cb.callout_flags |= PCRE2_CALLOUT_STARTMATCH;

		mb->start_used_ptr = start_match;
		mb->last_used_ptr = start_match;
		mb->match_call_count = 0;
		mb->end_offset_top = 0;
		mb->skip_arg_count = 0;

		rc = match(start_match, mb->start_code, match_data->ovector,
		           match_data->oveccount, re->top_bracket, frame_size, mb);

		if (mb->hitend && start_partial == NULL)
		{
			start_partial = mb->start_used_ptr;
			match_partial = start_match;
		}

		switch(rc)
		{
			/* If MATCH_SKIP_ARG reaches this level it means that a MARK that matched
			the SKIP's arg was not found. In this circumstance, Perl ignores the SKIP
			entirely. The only way we can do that is to re-do the match at the same
			point, with a flag to force SKIP with an argument to be ignored. Just
			treating this case as NOMATCH does not work because it does not check other
			alternatives in patterns such as A(*SKIP:A)B|AC when the subject is AC. */

			case MATCH_SKIP_ARG:
				new_start_match = start_match;
				mb->ignore_skip_arg = mb->skip_arg_count;
				break;

				/* SKIP passes back the next starting point explicitly, but if it is no
				greater than the match we have just done, treat it as NOMATCH. */

			case MATCH_SKIP:
				if (mb->verb_skip_ptr > start_match)
				{
					new_start_match = mb->verb_skip_ptr;
					break;
				}
				/* Fall through */

				/* NOMATCH and PRUNE advance by one character. THEN at this level acts
				exactly like PRUNE. Unset ignore SKIP-with-argument. */

			case MATCH_NOMATCH:
			case MATCH_PRUNE:
			case MATCH_THEN:
				mb->ignore_skip_arg = 0;
				new_start_match = start_match + 1;
#ifdef SUPPORT_UNICODE
			if (utf)
      ACROSSCHAR(new_start_match < end_subject, new_start_match,
        new_start_match++);
#endif
				break;

				/* COMMIT disables the bumpalong, but otherwise behaves as NOMATCH. */

			case MATCH_COMMIT:
				rc = MATCH_NOMATCH;
				goto ENDLOOP;

				/* Any other return is either a match, or some kind of error. */

			default:
				goto ENDLOOP;
		}

		/* Control reaches here for the various types of "no match at this point"
		result. Reset the code to MATCH_NOMATCH for subsequent checking. */

		rc = MATCH_NOMATCH;

		/* If PCRE2_FIRSTLINE is set, the match must happen before or at the first
		newline in the subject (though it may continue over the newline). Therefore,
		if we have just failed to match, starting at a newline, do not continue. */

		if (firstline && IS_NEWLINE(start_match)) break;

		/* Advance to new matching position */

		start_match = new_start_match;

		/* Break the loop if the pattern is anchored or if we have passed the end of
		the subject. */

		if (anchored || start_match > end_subject) break;

		/* If we have just passed a CR and we are now at a LF, and the pattern does
		not contain any explicit matches for \r or \n, and the newline option is CRLF
		or ANY or ANYCRLF, advance the match position by one more code unit. In
		normal matching start_match will aways be greater than the first position at
		this stage, but a failed *SKIP can cause a return at the same point, which is
		why the first test exists. */

		if (start_match > subject + start_offset &&
			start_match[-1] == CHAR_CR &&
			start_match < end_subject &&
			*start_match == CHAR_NL &&
			(re->flags & PCRE2_HASCRORLF) == 0 &&
			(mb->nltype == NLTYPE_ANY ||
				mb->nltype == NLTYPE_ANYCRLF ||
				mb->nllen == 2))
			start_match++;

		mb->mark = NULL;   /* Reset for start of next match attempt */
	}                  /* End of for(;;) "bumpalong" loop */

/* ==========================================================================*/

/* When we reach here, one of the following stopping conditions is true:

(1) The match succeeded, either completely, or partially;

(2) The pattern is anchored or the match was failed after (*COMMIT);

(3) We are past the end of the subject or the bumpalong limit;

(4) PCRE2_FIRSTLINE is set and we have failed to match at a newline, because
    this option requests that a match occur at or before the first newline in
    the subject.

(5) Some kind of error occurred.

*/

ENDLOOP:

/* Release an enlarged frame vector that is on the heap. */

	if (mb->match_frames != mb->stack_frames)
		mb->memctl.free(mb->match_frames, mb->memctl.memory_data);

/* Fill in fields that are always returned in the match data. */

	match_data->code = re;
	match_data->subject = subject;
	match_data->mark = mb->mark;
	match_data->matchedby = PCRE2_MATCHEDBY_INTERPRETER;

/* Handle a fully successful match. Set the return code to the number of
captured strings, or 0 if there were too many to fit into the ovector, and then
set the remaining returned values before returning. */

	if (rc == MATCH_MATCH)
	{
		match_data->rc = ((int)mb->end_offset_top >= 2 * match_data->oveccount)?
		                 0 : (int)mb->end_offset_top/2 + 1;
		match_data->startchar = start_match - subject;
		match_data->leftchar = mb->start_used_ptr - subject;
		match_data->rightchar = ((mb->last_used_ptr > mb->end_match_ptr)?
		                         mb->last_used_ptr : mb->end_match_ptr) - subject;
		return match_data->rc;
	}

/* Control gets here if there has been a partial match, an error, or if the
overall match attempt has failed at all permitted starting positions. Any mark
data is in the nomatch_mark field. */

	match_data->mark = mb->nomatch_mark;

/* For anything other than nomatch or partial match, just return the code. */

	if (rc != MATCH_NOMATCH && rc != PCRE2_ERROR_PARTIAL) match_data->rc = rc;

/* Handle a partial match. */

	else if (match_partial != NULL)
	{
		match_data->ovector[0] = match_partial - subject;
		match_data->ovector[1] = end_subject - subject;
		match_data->startchar = match_partial - subject;
		match_data->leftchar = start_partial - subject;
		match_data->rightchar = end_subject - subject;
		match_data->rc = PCRE2_ERROR_PARTIAL;
	}

/* Else this is the classic nomatch case. */

	else match_data->rc = PCRE2_ERROR_NOMATCH;

	return match_data->rc;
}

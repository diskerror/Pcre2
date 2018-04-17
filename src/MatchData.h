//
// Created by Reid Woodbury Jr on 4/16/18.
//

#ifndef DISKERROR_PCRE2_MATCHDATA_H
#define DISKERROR_PCRE2_MATCHDATA_H

class MatchData
{
protected:
	pcre2_match_data* ptr;

public:
	MatchData(pcre2_code *regex_compiled)
	{
		ptr = NULL;
		ptr = pcre2_match_data_create_from_pattern(regex_compiled, NULL);
		if (ptr == NULL)
			throw Php::Exception("match data returned null, could not obtain memory");
	}

	inline pcre2_match_data* operator()() const {return ptr;}

	~MatchData()
	{
		//  function checks for null input
		pcre2_match_data_free(ptr);
	}
};

#endif //DISKERROR_PCRE2_MATCHDATA_H

# Pcre2
This is an implimentation of the PCRE2 algorithm as a PHP extension. It compiles a regular expression as separate step from the comparison step. Run time is currently NOT faster than the builtin PCRE API... yet.

The extension makefile has been tested on Debian 8 with PHP 7.2, PCRE2 10.31, and PHP-CPP master branch of at least 2/9/18 which is required for PHP 7.2.

*Note:* passing PHP arrays by reference doesn't work in version 2.0.0 and later of PHP-CPP;

## Usage
In PHP, the PCRE function:
```
$subject = 'abacadabra';
$result = preg_replace('/a/u', ' ', $subject);
$this->assertEquals(' b c d br ', $result);

$result = (bool)preg_match('/a/u', $subject);
$this->assertTrue($result);

$matches = [];
$result = preg_match('/a/u', $subject, $matches);  //  1
$this->assertEquals(['a'], $matches);
```
Is equivalent to:
```
//  UTF is the default
$subject = 'abacadabra';
$replacer = new Diskerror\Pcre2\Replacer('a', ' ');
$result = $replacer->replace($subject);
$this->assertEquals(' b c d br ', $result);

$matcher = new Diskerror\Pcre2\Matcher('a');
$result = $matcher->hasMatch($subject);
$this->assertTrue($result);

$matches = $matcher->match($subject);  //  "$matcher" from above
$this->assertEquals(['a'], $matches);
```
This will perform replacements on multiple strings with only one compile step, and without needing them to be gathered into a single array.

## Requirements For Compiling
 It requires [PCRE2](http://www.pcre.org) and [PHP-CPP](http://www.php-cpp.com/) to be installed on the local system in order to build this extension as well as GCC, Make, the standard libraries, and the PHP development library.

If you are updating from an earlier version of this code be aware that the INI file for this extension is overwritten. Any setting there will be lost.

CentOS 6 requires at least devtoolset-2 to compile [PHP-CPP](http://www.php-cpp.com/).
```
 > cd /etc/yum.repos.d
 > wget http://people.centos.org/tru/devtools-2/devtools-2.repo
 > yum --enablerepo=testing-devtools-2-centos-6 install devtoolset-2-gcc devtoolset-2-gcc-c++
 > scl enable devtoolset-2 bash
```

### PCRE2
The PCRE2 source can be found [here](http://www.pcre.org).
```
 > ./configure --enable-jit --enable-newline-is-anycrlf
 > make
 > sudo make install
```

### PHP-CPP
The Copernica [PHP-CPP](https://github.com/CopernicaMarketingSoftware/PHP-CPP) library is used to build this extension.
```
 > make release
 > sudo make install
```
Just using ```make``` will create a slower debug version.

# LICENSE
This project has been changed to "unlicense". You are free to use it as you wish.

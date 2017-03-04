#ifndef FORMAT_SAVER_H_
#define FORMAT_SAVER_H_

#include <ios>

template<typename CharT>
class FormatSaver
{
public:
    FormatSaver(std::basic_ios<CharT>& ios) :
        stream(ios)
    {
        // save settings
        formatFlags = stream.flags();
        fill = stream.fill();
        precision = stream.precision();
        width = stream.width();
    }

    ~FormatSaver()
    {
        // restore settings
        stream.flags(formatFlags);
        stream.fill(fill);
        stream.precision(precision);
        stream.width(width);
    }

private:
    std::basic_ios<CharT>& stream;
    std::ios_base::fmtflags formatFlags;
    CharT fill;
    std::streamsize precision;
    std::streamsize width;
};

#endif // FORMAT_SAVER_H_

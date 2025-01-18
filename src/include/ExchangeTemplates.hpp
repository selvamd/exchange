#ifndef _EXCHANGE_TEMPLATES_HPP_
#define _EXCHANGE_TEMPLATES_HPP_

#include <string.h>
#include <chrono>
#include <string>
#include <arpa/inet.h>
#include <iostream>
#include <vector>
#include <algorithm>

namespace exchange
{

template <int maxsize>
class FixedString
{
    char sBuf[maxsize];

    private:
        void copy(const void * source)
        {
            memset(sBuf,0,maxsize);
            memcpy(sBuf, source, strnlen((char*)source, maxsize-1));
        }

        void copy(const FixedString& source)
        {
            copy(source.sBuf);
        }

    public:

        FixedString() {};
        FixedString(const char * data)  { copy(data); }
        FixedString(const std::string& data) { copy(data.c_str()); }
        FixedString(const FixedString& data) { copy(data); }

        void init() { memset(sBuf,0,maxsize); }

        size_t size() { return maxsize;  }

        void operator =(const std::string& data) { copy(data.c_str()); }
        void operator =(const char * data)  { copy(data); }

        bool operator ==(const char * data) const
        {
            return strncmp(sBuf,data, maxsize) == 0;
        }

        bool operator ==(const FixedString& data) const
        {
            return strncmp(sBuf,data.sBuf, maxsize) == 0;
        }

        bool operator < (const FixedString& data) const
        {
            return strncmp(sBuf,data.sBuf,maxsize) < 0;
        }

        int compareTo (const FixedString& data) const
        {
            return strncmp(sBuf,data.sBuf,maxsize);
        }

        int compareForIndex (const FixedString& data) const
        {
            if ( data.sBuf[0] == (char)0xFF && data.sBuf[1] == (char)0 )
                return 0;
            if ( sBuf[0] == (char)0xFF && sBuf[1] == (char)0 )
                return 0;
            return strncmp(sBuf,data.sBuf,maxsize);
        }

        int length () const
        {
            return strnlen(sBuf,maxsize);
        }

        const char * c_str() const
        {
            return sBuf;
        }

        friend std::ostream & operator<<(std::ostream & out, const FixedString & fstring)
        {
            out << fstring.sBuf;
            return out;
        }

        ~FixedString() {}
    };

    class Timestamp
    {
        uint64_t time;

        public:

            Timestamp(uint64_t t=0) : time(t) {}

            Timestamp(std::tm& ttm)
            {
                time_t ttime_t = mktime(&ttm);

                std::chrono::system_clock::time_point time_point_result = std::chrono::system_clock::from_time_t(ttime_t);
                const std::chrono::high_resolution_clock::duration since_epoch = time_point_result.time_since_epoch();
                time = std::chrono::duration_cast<std::chrono::microseconds>(since_epoch).count();
            }

            void operator =(const std::string& data) {
                if (data.empty())
                    init();
                else if (std::count(data.begin(), data.end(), '-') == 2 &&
                         std::count(data.begin(), data.end(), ' ') == 1 &&
                         std::count(data.begin(), data.end(), ':') == 2 &&
                         std::count(data.begin(), data.end(), '.') == 1) // format: YYYY-MM-DD HH:MM:SS.ssssss
                    mysql_string_to_micros(data);
                else if (std::count(data.begin(), data.end(), '-') == 2) // format: YYYY-MM-DD
                    date_to_micros(data);
                else // format: YYYYMMDD-HH:MM:SS.ssssss
                    string_to_micros(data);
            }

            void init() { time = 0; }

            void set()
            {
                std::chrono::system_clock::time_point time_point_now = std::chrono::system_clock::now();
                time = std::chrono::duration_cast<std::chrono::microseconds>(time_point_now.time_since_epoch()).count();
            }

            void set(uint64_t t) { time = t; }

            void operator =(const char * /* data */  )  { }

            bool operator ==(const Timestamp& data) const
            {
                return time == data.time;
            }

            bool operator < (const Timestamp& data) const
            {
                return time < data.time;
            }

            operator uint64_t() const { return time; }

            int compareTo (const Timestamp& data) const
            {
                if (time == data.time) return 0;
                return (time > data.time)? 1:-1;
            }

            int compareForIndex (const Timestamp& data) const
            {
                if (time == 0 || data.time == 0 ) return 0;
                if (time == data.time) return 0;
                return (time > data.time)? 1:-1;
            }

            friend std::ostream & operator<<(std::ostream & out, const Timestamp & ftime)
            {
                out << ftime.micros_to_string("%Y%m%d-%H:%M:%S", true);
                return out;
            }

            std::string toStringWithMicros() const
            {
                return toString(true);
            }

            std::string toString(const bool show_millis = false, const std::string & format = "%Y-%m-%d %H:%M:%S") const
            {
                return micros_to_string(format.c_str(), show_millis);
            }

            std::string getFixString() const
            {
                return micros_to_string("%Y%m%d");
            }

            /*
             *  Convenience Functions:
             */
            std::string getMysqlDate() const
            {
                return micros_to_string("%Y-%m-%d");
            }

            std::string getXmlApiDate() const
            {
                return micros_to_string("%Y%m%d");
            }

            std::string getXmlApiTime() const
            {
                return toString(true, "%H:%M:%S");
            }

            int64_t getInternalDate() const
            {
                return std::stol(micros_to_string("%Y%m%d"), NULL);
            }

            int64_t getInternalTime() const
            {
                return std::stol(micros_to_string("%H%M%S"), NULL);
            }

            void addDays(int32_t days)
            {
                std::chrono::hours hours(days * 24);
                std::chrono::microseconds micros = std::chrono::duration_cast<std::chrono::microseconds>(hours);

                time += micros.count();
            }

        private:
            void date_to_micros(const std::string &str)
            {
                int yyyy, mm, dd;

                //YYYYMMDD-HH:MM:SS.ssssss
                char scanf_format[] = "%4d-%2d-%2d";

                sscanf(str.c_str(), scanf_format, &yyyy, &mm, &dd);

                tm ttm = tm();
                ttm.tm_year = yyyy - 1900; // Year since 1900
                ttm.tm_mon = mm - 1; // Month since January
                ttm.tm_mday = dd; // Day of the month [1-31]
                ttm.tm_hour = 0; // Hour of the day [00-23]
                ttm.tm_min = 0;
                ttm.tm_sec = 0;

                time_t ttime_t = mktime(&ttm);

                std::chrono::system_clock::time_point time_point_result = std::chrono::system_clock::from_time_t(ttime_t);
                const std::chrono::high_resolution_clock::duration since_epoch = time_point_result.time_since_epoch();
                time = std::chrono::duration_cast<std::chrono::microseconds>(since_epoch).count();
            }

            void string_to_micros(const std::string &str)
            {
                int yyyy, mm, dd, HH, MM, SS, fff;

                //YYYYMMDD-HH:MM:SS.ssssss
                char scanf_format[] = "%4d%2d%2d-%2d:%2d:%2d.%6d";

                sscanf(str.c_str(), scanf_format, &yyyy, &mm, &dd, &HH, &MM, &SS, &fff);

                tm ttm = tm();
                ttm.tm_year = yyyy - 1900; // Year since 1900
                ttm.tm_mon = mm - 1; // Month since January
                ttm.tm_mday = dd; // Day of the month [1-31]
                ttm.tm_hour = HH; // Hour of the day [00-23]
                ttm.tm_min = MM;
                ttm.tm_sec = SS;

                time_t ttime_t = mktime(&ttm);

                std::chrono::system_clock::time_point time_point_result = std::chrono::system_clock::from_time_t(ttime_t);
                time_point_result += std::chrono::microseconds(fff);
                const std::chrono::high_resolution_clock::duration since_epoch = time_point_result.time_since_epoch();
                time = std::chrono::duration_cast<std::chrono::microseconds>(since_epoch).count();
            }

            void mysql_string_to_micros(const std::string &str)
            {
                int yyyy, mm, dd, HH, MM, SS, fff;

                char scanf_format[] = "%4d-%2d-%2d %2d:%2d:%2d.%6d";

                sscanf(str.c_str(), scanf_format, &yyyy, &mm, &dd, &HH, &MM, &SS, &fff);

                tm ttm = tm();
                ttm.tm_year = yyyy - 1900; // Year since 1900
                ttm.tm_mon = mm - 1; // Month since January
                ttm.tm_mday = dd; // Day of the month [1-31]
                ttm.tm_hour = HH; // Hour of the day [00-23]
                ttm.tm_min = MM;
                ttm.tm_sec = SS;
                ttm.tm_isdst = -1; // RLujo - previously was bumping up by 1 hour due to being in DST

                time_t ttime_t = mktime(&ttm);

                std::chrono::system_clock::time_point time_point_result = std::chrono::system_clock::from_time_t(ttime_t);
                time_point_result += std::chrono::microseconds(fff);
                const std::chrono::high_resolution_clock::duration since_epoch = time_point_result.time_since_epoch();
                time = std::chrono::duration_cast<std::chrono::microseconds>(since_epoch).count();
            }

            std::string micros_to_string(const std::string & date_time_format, const bool show_millis = false) const
            {
                std::chrono::system_clock::time_point tp;
                tp += std::chrono::microseconds(time);

                auto ttime_t = std::chrono::system_clock::to_time_t(tp);
                auto tp_sec = std::chrono::system_clock::from_time_t(ttime_t);
                std::chrono::microseconds ms = std::chrono::duration_cast<std::chrono::microseconds>(tp - tp_sec);

                std::tm * ttm = localtime(&ttime_t);

                char time_str[50];

                strftime(time_str, sizeof(time_str), date_time_format.c_str(), ttm);

                std::string result(time_str);
                if (show_millis)
                {
                    result.append(".");
                    long num = 1000000 + ms.count();
                    result.append(std::to_string(num).substr(1));
                }

                return result;
            }
    };

/*
 *  Wrapper class around enumerations that provides print, translation,
 *  assignment and compare functions for enum data types.
 */
template <typename enumtype>
class EnumData
{
	private:
        enumtype sValue;

        void assignFromString(const std::string& s)
        {
            const std::vector<std::string>& enumVec = getEnumVector(sValue);
            for (size_t i=0;i < enumVec.size();i++ )
            {
                if (s == enumVec[i].substr(2))
                {
                    sValue = (enumtype)i;
                    return;
                }
            }
        }

        void assignFromChar(const char c)
        {
            const std::vector<std::string>& enumVec = getEnumVector(sValue);
            for (size_t i=0;i < enumVec.size();i++ )
            {
                if (c == enumVec[i][0])
                {
                    sValue = (enumtype)i;
                    return;
                }
            }
        }

        std::string getString() const
        {
            const std::vector<std::string>& enumVec = getEnumVector(sValue);
            if ((size_t)sValue >= enumVec.size())
                return "x-Unknown";
            return enumVec[(size_t)sValue];
        }

	public:

        void init()
        {
            sValue = (enumtype)0xFF;
        }

        EnumData()
        {
            init();
        };

        EnumData(const std::string& data)
        {
            init();
            assignFromString(data);
        }

        EnumData(const int8_t& data)
        {
            init();
            sValue = (enumtype)data;
        }

        EnumData(const char data)
        {
            init();
            assignFromChar(data);
        }

        EnumData(const EnumData& data)
        {
            init();
            sValue = data.sValue;
        }

        EnumData(const enumtype& data)
        {
            init();
            sValue = data;
        }

        bool isValid() const
        {
            const std::vector<std::string>& enumVec = getEnumVector(sValue);
            if ((size_t)sValue >= enumVec.size())
                return false;
            return true;
        }

		size_t size() const
        {
            return sizeof(enumtype);
        }

        char getFixCode() const
        {
            return getString().c_str()[0];
        }

        std::string toString() const
        {
            return getString().substr(2);
        }

        void operator =(const int8_t& data)
        {
            sValue = (enumtype)data;
        }

        void operator =(const std::string& data)
        {
            assignFromString(data);
        }

		void operator =(const enumtype data)
        {
            sValue = data;
        }

		void operator =(const EnumData<enumtype> data)
        {
            sValue = data.sValue;
        }

        enumtype operator ()() const
        {
            return sValue;
        }

        enumtype getEnum() const
        {
            return sValue;
        }

		bool operator < (const enumtype& data) const
		{
			return (int8_t)sValue < (int8_t)data();
		}

		int compareTo (const EnumData& data) const
		{
            if (sValue == data()) return 0;
            return (sValue < data())? -1:1;
		}

        int compareForIndex (const EnumData& data) const
        {
            enumtype test = (enumtype)0xFF;
            if ( test == sValue || test == data.sValue) return 0;
            if (sValue == data()) return 0;
            return (sValue < data())? -1:1;
        }

	friend std::ostream & operator << (std::ostream & out, const EnumData & data)
	{
		out << data.toString();
		return out;
	}

	~EnumData() {}
};

template<typename enumVal>
inline std::ostream& print(std::ostream &out, enumVal val)
{
    EnumData<enumVal> data(val);
    out << data.toString();
    return out;
}


template <typename data>
int32_t convertStringToEnum(data &value, const std::string & str)
{
    EnumData<data> enumObj(str);
    if (!enumObj.isValid())
        return -1;
    value = enumObj();
    return 0;

}

template <typename data>
int32_t convertEnumToString(const data value, std::string & str)
{
    EnumData<data> enumObj(value);
    if (!enumObj.isValid())
    return -1;
    str.assign(enumObj.toString());
    return 0;

}

template <typename numtype>
int32_t convertStringToNum(numtype &field, const char * value)
{
    long long num = strtol(value, NULL, 10);
    /*
     * TODO: Review
     *
    char* endptr = nullptr;
    long long num = strtol(value, &endptr, 10);

    if (num == 0 &&
        endptr == value)
    {
        //No valid digits in the value
        return -1;
    }

    if (endptr != nullptr &&
        endptr[0] != 0)
    {
        //There is junk after the valid digits in the value
        return -1;
    }
    */

    if (num < (long long)std::numeric_limits<numtype>::min() && num > (long long)std::numeric_limits<numtype>::max())
    {
        std::cout << " Conversion error. Overflow " << std::endl;
        return -1;
    }
    field = (numtype)num;
    return 0;
}

template <typename numtype>
int32_t convertPrice(numtype &field, const char * value, int32_t price_scale)
{
    double num = strtod(value, NULL) * price_scale;
    /*
     * TODO: Review
     *
    char* endptr = nullptr;
    double num = strtod(value, &endptr) * price_scale;

    if (endptr != nullptr &&
        endptr[0] != 0)
    {
        //There is non-numeric junk in the value
        return -1;
    }
    */
    num += 0.5;
    field = (numtype)num;
    return 0;
}

inline std::string quoteString(const std::string & str)
{
    char result[100];
    memset(result,0,100);
    sprintf(result,"\"%s\"", str.c_str());
    return result;
}

#ifndef hton64
#define hton64(x) ((1==htonl(1)) ? (x) : ((uint64_t)htonl((x) & 0xFFFFFFFF) << 32) | htonl((x) >> 32))
#endif

#ifndef ntoh64
#define ntoh64(x) ((1==ntohl(1)) ? (x) : ((uint64_t)ntohl((x) & 0xFFFFFFFF) << 32) | ntohl((x) >> 32))
#endif

template <typename EnumType>
EnumType hton_enum(EnumType x)
{
    return (EnumType) htonl((int32_t) x);
}

template <typename EnumType>
EnumType ntoh_enum(EnumType x)
{
    return (EnumType) ntohl((int32_t) x);
}

#ifdef DECLARE_STRING_FIELD
#undef DECLARE_STRING_FIELD
#endif

#define DECLARE_STRING_FIELD(FieldName, CamelCaseName, FieldLen)        \
                                                                        \
    char FieldName [FieldLen + 1];                                      \
                                                                        \
    const char * get##CamelCaseName() const {                           \
        return (const char*) FieldName;                                 \
    }                                                                   \
                                                                        \
    std::string getDBStr##CamelCaseName() const {                       \
        return exchange::quoteString(FieldName);                        \
    }                                                                   \
                                                                        \
    std::string getFieldName##CamelCaseName() const                     \
    {                                                                   \
        return std::string(#FieldName);                                 \
    }                                                                   \
                                                                        \
    void print##CamelCaseName(std::ostream& o) const {                  \
        o <<  "," << #FieldName << "="                                  \
                  << FieldName;                                         \
    }                                                                   \
                                                                        \
    int32_t set##CamelCaseName(const char * value) {                    \
        if (value == nullptr ||                                         \
            strnlen(value,FieldLen+1) > FieldLen )                      \
        {                                                               \
            std::cout << #FieldName <<                                  \
                    " invalid " << std::endl;                           \
            return -1;                                                  \
        }                                                               \
        strncpy(FieldName, value, FieldLen);                            \
        return 0;                                                       \
    }

#ifdef DECLARE_NUM_FIELD
#undef DECLARE_NUM_FIELD
#endif

#define DECLARE_NUM_FIELD(FieldName, CamelCaseName, FieldType)          \
                                                                        \
    FieldType FieldName;                                                \
                                                                        \
    FieldType   get##CamelCaseName() const {                            \
        return FieldName;                                               \
    }                                                                   \
                                                                        \
    std::string getDBStr##CamelCaseName() const {                       \
        return std::to_string(FieldName);                               \
    }                                                                   \
                                                                        \
    std::string getFieldName##CamelCaseName() const                     \
    {                                                                   \
        return std::string(#FieldName);                                 \
    }                                                                   \
                                                                        \
    void print##CamelCaseName(std::ostream& o) const {                  \
        o <<  "," << #FieldName << "=" << FieldName;                    \
    }                                                                   \
                                                                        \
    int32_t set##CamelCaseName(FieldType value) {                       \
        FieldName = value;                                              \
        return 0;                                                       \
    }                                                                   \
                                                                        \
    int32_t set##CamelCaseName(const char * value) {                    \
        return exchange::convertStringToNum<FieldType>(FieldName, value); \
    }

#ifdef DECLARE_PRICE_FIELD
#undef DECLARE_PRICE_FIELD
#endif

#define DECLARE_PRICE_FIELD(FieldName, CamelCaseName, FieldType, PriceScale) \
                                                                        \
    FieldType FieldName;                                                \
                                                                        \
    FieldType   get##CamelCaseName() const {                            \
        return FieldName;                                               \
    }                                                                   \
                                                                        \
    std::string getDBStr##CamelCaseName() const {                       \
        double d = ((double)FieldName/(double)PriceScale);              \
        return std::to_string(d);                                       \
    }                                                                   \
                                                                        \
    std::string getFieldName##CamelCaseName() const                     \
    {                                                                   \
        return std::string(#FieldName);                                 \
    }                                                                   \
                                                                        \
    void print##CamelCaseName(std::ostream& o) const {                  \
        o <<  "," << #FieldName << "=" << FieldName;                    \
    }                                                                   \
                                                                        \
    int32_t set##CamelCaseName(FieldType value) {                       \
        FieldName = value;                                              \
        return 0;                                                       \
    }                                                                   \
                                                                        \
    int32_t set##CamelCaseName(const char * value) {                    \
        return exchange::convertPrice(FieldName, value, PriceScale);    \
    }

#ifdef DECLARE_TIME_FIELD
#undef DECLARE_TIME_FIELD
#endif

#define DECLARE_TIME_FIELD(FieldName, CamelCaseName, FieldType)         \
                                                                        \
    FieldType FieldName;                                                \
                                                                        \
    FieldType   get##CamelCaseName() const {                            \
        return FieldName;                                               \
    }                                                                   \
    std::string getDBStr##CamelCaseName() const {                       \
        exchange::Timestamp time(FieldName);                            \
        return exchange::quoteString(time.toString());                  \
    }                                                                   \
                                                                        \
    std::string getFieldName##CamelCaseName() const                     \
    {                                                                   \
        return std::string(#FieldName);                                 \
    }                                                                   \
                                                                        \
    void print##CamelCaseName(std::ostream& o) const {                  \
        exchange::Timestamp time(FieldName);                            \
        o <<  "," << #FieldName << "=" << time;                         \
    }                                                                   \
                                                                        \
    int32_t set##CamelCaseName(FieldType value) {                       \
        FieldName = value;                                              \
        return 0;                                                       \
    }                                                                   \
                                                                        \
    int32_t set##CamelCaseName(const char * value) {                    \
        exchange::Timestamp time;                                       \
        time = value;                                                   \
        FieldName = time;                                               \
        return ( FieldName >= 0 )? 0:-1;                                \
    }

#ifdef DECLARE_ENUM_FIELD
#undef DECLARE_ENUM_FIELD
#endif

#define DECLARE_ENUM_FIELD(FieldName, CamelCaseName, FieldType)         \
                                                                        \
    FieldType FieldName;                                                \
                                                                        \
    FieldType   get##CamelCaseName() const {                            \
        return FieldName;                                               \
    }                                                                   \
    std::string getDBStr##CamelCaseName() const {                       \
        exchange::EnumData<FieldType> f = FieldName;                    \
        return exchange::quoteString(f.toString());                     \
    }                                                                   \
                                                                        \
    std::string getFieldName##CamelCaseName() const                     \
    {                                                                   \
        return std::string(#FieldName);                                 \
    }                                                                   \
    void print##CamelCaseName(std::ostream& o) const {                  \
        exchange::EnumData<FieldType> f = FieldName;                    \
        o <<  "," << #FieldName << "=" << f.toString();                 \
    }                                                                   \
                                                                        \
    int32_t set##CamelCaseName(                                         \
        exchange::EnumData<FieldType> value) {                          \
        FieldName = value();                                            \
        return 0;                                                       \
    }                                                                   \
                                                                        \
    int32_t set##CamelCaseName(FieldType value) {                       \
        FieldName = value;                                              \
        return 0;                                                       \
    }                                                                   \
                                                                        \
    int32_t set##CamelCaseName(const char * value) {                    \
        exchange::EnumData<FieldType> f(value);                         \
        if ( !f.isValid() )                                             \
        {                                                               \
            std::cout << "Error setting " << #FieldName << std::endl;   \
            return -1;                                                  \
        }                                                               \
        FieldName = f();                                                \
        return 0;                                                       \
    }                                                                   \
    const std::string toString##CamelCaseName() const {                 \
        exchange::EnumData<FieldType> f = FieldName;                    \
        return f.toString();                                            \
    }
}

#endif

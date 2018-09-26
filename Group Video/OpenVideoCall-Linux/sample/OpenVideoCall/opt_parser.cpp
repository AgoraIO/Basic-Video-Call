#include <cassert>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>
#include <cstring>

#if (defined(_WIN32) || defined(_WIN64)) && !defined(__GNUC__)
enum {ERROR=-1, INFO=0, WARNING, FATAL};
#define LOG(level, fmt, ...) fprintf(stderr, #level fmt "\n", __VA_ARGS__)
#define strtoll _strtoi64
#define strtoull _strtoui64
#else
#include <getopt.h>
#endif

#include "opt_parser.h"

using namespace std;

#if defined(_WIN32) && !defined(__GNUC__)

enum {no_argument = 0, required_argument, optional_argument};

int optind = 0;
const char *optarg = NULL;

struct option {
    const char *name;
    int has_arg;
    int *flag;
    int val;
};

int getopt_long_only(int argc, char *const argv[], const char *short_opts,
        const option *long_opts, int *index) {
    if (short_opts && *short_opts != '\0') {
        //LOG(ERROR, "short options have not been implemented yet!");
        return -1;
    }

    optind = optind == 0 ? 1 : optind;
    optarg = NULL;

    if (optind >= argc)
        return -1;

    const char *a = argv[optind];
    if (a[0] != '-' || a[1] != '-')
        return -1;

    a += 2;

    int i = 0;

    for (; long_opts[i].name != NULL; ++i) {
        if (!strcmp(long_opts[i].name, a)) {
            ++optind;
            *index = i;
            return 0;
        }
    }

    return '?';
}

#endif


opt_parser::opt_parser():sequence_(0)
{}
bool opt_parser::insert_long_opt(internal_opt &opt, const char *long_opt) {
    if (!long_opt) {
        //LOG(ERROR, "A full parameter should be supplied!");
        return false;
    }

    if (long_opts_.count(long_opt) > 0) {
        //LOG(ERROR, "{%s} has been occupied yet!", long_opt);
        return false;
    }
    if(opt.help == NULL || !strlen(opt.help))
        opt.help = "NA";

    long_opts_[long_opt] = opt;
    return true;
}

// bool opt_parser::add_short_arg(bool *store, char short_arg) {
//   *store = false;
//
//   internal_opt arg = {kBool, {store}};
//   return insert_short_arg(arg, short_arg);
// }
//
// bool opt_parser::add_short_arg(int *store, char short_arg) {
//   internal_opt arg;
//   arg.type = kInt;
//   arg.int_ptr = store;
//
//   return insert_short_arg(arg, short_arg);
// }
//
// bool opt_parser::add_short_arg(string *store, char short_arg) {
//   internal_opt arg;
//   arg.type = kString;
//   arg.string_ptr = store;
//
//   return insert_short_arg(arg, short_arg);
// }

bool opt_parser::add_long_opt(const char *long_opt, bool *store,
        const char *help, opt_parser::opt_type optParamType)
{
    internal_opt arg;
    arg.type = kBool;
    arg.bool_ptr = store;
    arg.help = help;
    arg.optType = optParamType;
    arg.seq = ++sequence_;
    return insert_long_opt(arg, long_opt);
}

bool opt_parser::add_long_opt(const char *long_opt, int32_t *store,
        const char *help, opt_parser::opt_type optParamType)
{
    internal_opt arg;
    arg.type = kInt32;
    arg.int32_ptr = store;
    arg.help = help;
    arg.optType = optParamType;
    arg.seq = ++sequence_;
    return insert_long_opt(arg, long_opt);
}

bool opt_parser::add_long_opt(const char *long_opt, uint32_t *store,
        const char *help, opt_parser::opt_type optParamType)
{
    internal_opt arg;
    arg.type = kUInt32;
    arg.uint32_ptr = store;
    arg.help = help;
    arg.optType = optParamType;
    arg.seq = ++sequence_;
    return insert_long_opt(arg, long_opt);
}

bool opt_parser::add_long_opt(const char *long_opt, int64_t *store,
        const char *help, opt_parser::opt_type optParamType)
{
    internal_opt arg;
    arg.type = kInt64;
    arg.int64_ptr = store;
    arg.help = help;
    arg.optType = optParamType;
    arg.seq = ++sequence_;
    return insert_long_opt(arg, long_opt);
}

bool opt_parser::add_long_opt(const char *long_opt, uint64_t *store,
        const char *help, opt_parser::opt_type optParamType)
{
    internal_opt arg;
    arg.type = kUInt64;
    arg.uint64_ptr = store;
    arg.help = help;
    arg.optType = optParamType;
    arg.seq = ++sequence_;
    return insert_long_opt(arg, long_opt);
}

bool opt_parser::add_long_opt(const char *long_opt, double *store,
        const char *help, opt_parser::opt_type optParamType)
{
    internal_opt arg;
    arg.type = kDouble;
    arg.double_ptr = store;
    arg.help = help;
    arg.optType = optParamType;
    arg.seq = ++sequence_;
    return insert_long_opt(arg, long_opt);
}

bool opt_parser::add_long_opt(const char *long_opt, string *store,
        const char *help, opt_parser::opt_type optParamType)
{
    internal_opt arg;
    arg.type = kString;
    arg.string_ptr = store;
    arg.help = help;
    arg.optType = optParamType;
    arg.seq = ++sequence_;
    return insert_long_opt(arg, long_opt);
}

bool opt_parser::fill_arg(const char *opt_name, const internal_opt &opt,
        const char *opt_arg) {

    if (opt_arg == NULL) {
        //LOG(ERROR, "No argument available for %s", opt_name);
        return false;
    }

    char *end_ptr = NULL;

    switch (opt.type) {
        case kBool: {
                        unsigned long n = strtoul(opt_arg, &end_ptr, 10);
                        if (*end_ptr != '\0') {
                            //LOG(ERROR, "Invalid integer argument: %s", opt_arg);
                            return false;
                        }
                        *opt.bool_ptr = n? true:false;
                    }
                    break;

        case kInt32:  {
                          long n = strtol(opt_arg, &end_ptr, 10);
                          if (*end_ptr != '\0') {
                              // LOG(ERROR, "Invalid integer argument: %s", opt_arg);
                              return false;
                          }
                          *opt.int32_ptr = int32_t(n);
                          break;
                      }
        case kUInt32:  {
                           unsigned long n = strtoul(opt_arg, &end_ptr, 10);
                           if (*end_ptr != '\0') {
                               // LOG(ERROR, "Invalid integer argument: %s", opt_arg);
                               return false;
                           }
                           *opt.uint32_ptr = uint32_t(n);
                           break;
                       }
        case kInt64:  {
                          long long n = strtoll(opt_arg, &end_ptr, 10);
                          if (*end_ptr != '\0') {
                              // LOG(ERROR, "Invalid integer argument: %s", opt_arg);
                              return false;
                          }
                          *opt.int64_ptr = int64_t(n);
                          break;
                      }
        case kUInt64:  {
                           unsigned long long n = strtoull(opt_arg, &end_ptr, 10);
                           if (*end_ptr != '\0') {
                               // LOG(ERROR, "Invalid integer argument: %s", opt_arg);
                               return false;
                           }
                           *opt.uint64_ptr = uint64_t(n);
                           break;
                       }
        case kDouble: {
                          double n = strtod(opt_arg, &end_ptr);
                          if (*end_ptr != '\0') {
                              // LOG(ERROR, "Invalid double argument: %s", opt_arg);
                              return false;
                          }
                          *opt.double_ptr = n;
                          break;
                      }
        case kString: {
                          *opt.string_ptr = opt_arg;
                          break;
                      }
        default: assert(false); break;
    }

    return true;
}

void opt_parser::save_to_exopts()const
{
    typedef unordered_map<const char*, internal_opt>::const_iterator It;
    for(It iter=long_opts_.begin(); iter != long_opts_.end(); ++ iter)
    {
        ex_internal_opt ex_opt;
        ex_opt.long_opt = iter->first;
        ex_opt.stru_opt = iter->second;
        int key = iter->second.seq;
        ex_long_opts_[key]=ex_opt;
    }
}
bool opt_parser::parse_opts(int argc, char* const argv[]) {
    //  vector<char> options;
    //  options.reserve(short_args_.size() + 1);
    //
    //  unordered_map<char, internal_opt>::const_iterator it;
    //  for (it = short_args_.begin(); it != short_args_.end(); ++it) {
    //    options.push_back(it->first);
    //    if (it->second.type != kBool)
    //      options.push_back(':');
    //  }
    //
    //  options.push_back('\0');

    vector<option> long_opt;
    long_opt.reserve(long_opts_.size() + 1);

    unordered_map<const char *, internal_opt>::const_iterator f;
    for (f = long_opts_.begin(); f != long_opts_.end(); ++f) {
        option arg = {f->first, f->second.optType == opt_argu ?
            optional_argument : required_argument, 0, 0};
        long_opt.push_back(arg);
    }

    option end_indicator = {NULL, 0, NULL, 0};
    long_opt.push_back(end_indicator);
    int index, ret;

    // NOTE(liuyong): MUST initialize this variable first!
    optind = 1;
    optarg = NULL;

    while ((ret = getopt_long_only(argc, argv, "", &long_opt[0],
                    &index)) == 0 || (ret > 0 && ret != '?')) {
        if (ret == 0) { // handle long options
            const option &opt = long_opt[index];
            const internal_opt &a = long_opts_[opt.name];
            const char *arg = optarg;
            if (!arg && optind < argc) {
                arg = argv[optind];
                ++optind;
            }

            if (!fill_arg(opt.name, a, arg)) {
                optind = 0;
                return false;
            }
        }/* else if (short_args_.count(ret) > 0) { // handle short options
            const internal_opt &a = short_args_[ret];
            if (!fill_arg(a, optarg))
            return false;
            } */
    }

    index = optind;
    optind = 0;


    if (index < argc) {
        //LOG(ERROR, "Unrecognized option argument: %s", argv[index]);
        return false;
    }

    if (ret != -1) {
        //LOG(ERROR, "Unregconized option argument %s", argv[index - 1]);
        return false;
    }

    return true;
}

void opt_parser::clear() {
    //  short_args_.clear();
    long_opts_.clear();
    ex_long_opts_.clear();
}

void opt_parser::print_usage(const char *exec_file, ostream &sout) const {
    sout << "Usage: \n  " << exec_file << " ";
    size_t fileLength = strlen(exec_file);
    save_to_exopts();
    typedef std::map<int, ex_internal_opt>::const_iterator It;
    for (It f = ex_long_opts_.begin(); f != ex_long_opts_.end(); ++f) {
        sout << "--" << f->second.long_opt << " ";
        const internal_opt &opt = f->second.stru_opt;
        const char *opt_arg = NULL;

        switch (opt.type) {
            case kBool: continue;
            case kInt32:
                        opt_arg = "INTEGER32";
                        break;
            case kUInt32:
                        opt_arg = "UINTEGER32";
                        break;
            case kInt64:
                        opt_arg = "INTEGER64";
                        break;
            case kUInt64:
                        opt_arg = "UINTEGER64";
                        break;
            case kDouble:
                        opt_arg = "DOUBLE";
                        break;
            case kString:
                        opt_arg = "STRING";
                        break;
            default:
                        assert(false);
                        break;
        }

        sout << opt_arg << " ";
    }

    sout << endl;

    for (It f = ex_long_opts_.begin(); f != ex_long_opts_.end(); ++f) {
        sout<<"   ";
        for(size_t i = 0; i < fileLength; i++) sout<<" ";
        sout << "--" << f->second.long_opt << "     ("<<f->second.stru_opt.help<<")"<<endl;
    }
}



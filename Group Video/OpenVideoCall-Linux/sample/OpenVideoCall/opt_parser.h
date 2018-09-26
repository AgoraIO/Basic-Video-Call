#pragma once

#include <cstdint>
#include <iostream>
#include <string>
#include <unordered_map>
#include <map>

class opt_parser {
    public:
        enum opt_type {
            no_argu=0,
            require_argu=1,
            opt_argu=2,
        };

    private:
        enum pointer_type {kBool, kInt32, kUInt32, kInt64, kUInt64, kDouble, kString,
        };

        struct internal_opt {
            pointer_type type;
            union {
                bool *bool_ptr;
                int32_t *int32_ptr;
                uint32_t *uint32_ptr;
                int64_t *int64_ptr;
                uint64_t *uint64_ptr;
                double *double_ptr;
                std::string *string_ptr;
            };
            const char *help;
            opt_type optType;
            int seq;
        };
        struct ex_internal_opt{
            internal_opt stru_opt;
            const char* long_opt;
        };
    public:
        opt_parser();

        bool add_long_opt(const char *long_opt, bool *store, const char *help, 
                opt_parser::opt_type optParamType = opt_parser::opt_argu);
        bool add_long_opt(const char *long_opt, int32_t *store, const char *help, 
                opt_parser::opt_type optParamType = opt_parser::opt_argu);
        bool add_long_opt(const char *long_opt, uint32_t *store, const char *help, 
                opt_parser::opt_type optParamType = opt_parser::opt_argu);
        bool add_long_opt(const char *long_opt, int64_t *store, const char *help, 
                opt_parser::opt_type optParamType = opt_parser::opt_argu);
        bool add_long_opt(const char *long_opt, uint64_t *store, const char *help, 
                opt_parser::opt_type optParamType = opt_parser::opt_argu);
        bool add_long_opt(const char *long_opt, double *store, const char *help, 
                opt_parser::opt_type optParamType = opt_parser::opt_argu);
        bool add_long_opt(const char *long_opt, std::string *store, const char *help, 
                opt_parser::opt_type optParamType = opt_parser::opt_argu);

        bool parse_opts(int argc, char *const argv[]);
        void clear();
        void save_to_exopts()const;
        void print_usage(const char *exec_file, std::ostream &sout) const;
    private:
        bool insert_long_opt(internal_opt &opt, const char *long_opt);

        static bool fill_arg(const char *opt_name, const internal_opt &opt,
                const char *opt_arg);

    private:
        std::unordered_map<const char *, internal_opt> long_opts_;
        mutable std::map<int, ex_internal_opt> ex_long_opts_;
        int sequence_;
};



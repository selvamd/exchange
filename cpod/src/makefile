##
## Global makefile
## 
## Simplest approach at this point. 
##

include make.include

## Build the code generators first so that their outputs
## are available for subsequent steps
SUBDIRS = generated_code/libs libs gateway market_data test_tools ecg ops tdd gen_tdd qa configuration data_loaders feed_handlers fmv_calculator email_server push_notification tcp_capture stream_archive market_maker
PAR_SUBDIRS = gateway market_data test_tools ecg ops tdd gen_tdd qa configuration data_loaders feed_handlers fmv_calculator email_server push_notification tcp_capture stream_archive market_maker
PROC_SUBDIRS = gateway market_data test_tools ecg ops tdd gen_tdd qa configuration data_loaders feed_handlers fmv_calculator email_server push_notification tcp_capture stream_archive market_maker
BUILD_SUBDIRS = $(SUBDIRS:%=build-%)
COV_SUBDIRS = $(SUBDIRS:%=cov-%)
PROF_SUBDIRS = $(SUBDIRS:%=prof-%)
CLEANUP_SUBDIRS = $(SUBDIRS:%=cleanup-%)
PARALLEL_SUBDIRS = $(PAR_SUBDIRS:%=parallel-%)
EXCHANGE_SUBDIRS = $(PROC_SUBDIRS:%=exchange-%)
INSTALL_SUBDIRS = $(PROC_SUBDIRS:%=install-%)
UT_SUBDIRS = $(SUBDIRS:%=utest-%)
UT_COV_SUBDIRS = $(SUBDIRS:%=utest_cov-%)

.PHONY: all create_dirs build profile coverage clean clean_dirs clean_src gen_libs_clean gen_libs_all gen_libs_all_uts gen_libs_exchange libs_clean libs_all libs_all_uts libs_exchange $(SUBDIRS) $(PAR_SUBDIRS) $(PROC_SUBDIRS) $(BUILD_SUBDIRS) $(COV_SUBDIRS) $(CLEANUP_SUBDIRS) $(PARALLEL_SUBDIRS) $(EXCHANGE_SUBDIRS) $(INSTALL_SUBDIRS) $(UT_SUBDIRS) $(UT_COV_SUBDIRS)

all: create_dirs gen_libs_clean gen_libs_all libs_clean libs_all $(PARALLEL_SUBDIRS)
build: $(BUILD_SUBDIRS)
profile : create_dirs $(PROF_SUBDIRS)
coverage : create_dirs $(COV_SUBDIRS)
clean: clean_dirs $(CLEANUP_SUBDIRS)
clean_src : $(CLEANUP_SUBDIRS)
install : $(INSTALL_SUBDIRS)
run_ut : $(UT_SUBDIRS)
run_ut_cov : $(UT_COV_SUBDIRS)
exchange : gen_libs_clean gen_libs_exchange libs_clean libs_exchange $(EXCHANGE_SUBDIRS)
quick:
	make exchange -s -j 4

create_dirs:
	if test -d ../build; then echo ../build exists; else  mkdir ../build; fi
	if test -d ../build/libs; then echo ../build/libs exists; else  mkdir ../build/libs; fi
	if test -d ../build/bin; then echo ../build/bin exists; else  mkdir ../build/bin; fi
	if test -d ../build/conf; then echo ../build/conf exists; else  mkdir ../build/conf; fi
	if test -d ../build/data; then echo ../build/data exists; else  mkdir ../build/data; fi
	if test -d ../build/html; then echo ../build/html exists; else  mkdir ../build/html; fi
	if test -d ../build/data/ops; then echo ../build/data/ops exists; else  mkdir ../build/data/ops; fi
	if test -d ../build/log; then echo ../build/log exists; else  mkdir ../build/log; fi
	if test -d ../build/log/junit; then echo ../build/log/junit exists; else  mkdir ../build/log/junit; fi
	if test -d ../build/script; then echo ../build/script exists; else  mkdir ../build/script; fi
	if test -d ../build/include; then echo ../build/include exists; else  mkdir ../build/include; fi
	if test -d ../build/exchange_instances; then echo ../build/exchange_instances exists; else  mkdir ../build/exchange_instances; fi

clean_dirs:
	if test -d ../build/lib; then rm -rf ../build/lib/*; fi
	if test -d ../build/bin; then rm -rf ../build/bin/*; fi
	if test -d ../build/conf; then rm -rf ../build/conf/*; fi
	if test -d ../build/data/ops; then rm -rf ../build/data/ops/*; fi
	if test -d ../build/data; then rm -rf ../build/data/*; fi
	if test -d ../build/html; then rm -rf ../build/html/*; fi
	if test -d ../build/log; then rm -rf ../build/log/*; fi
	if test -d ../build/script; then rm -rf ../build/script/*; fi
	if test -d ../build/include; then rm -rf ../build/include/*; fi
	if test -d ../build/exchange_instances; then rm -rf ../build/exchange_instances/*; fi

gen_libs_clean :
	- echo -e "$(OK_COLOR)Cleaning gen libs$(NO_COLOR)"
	$(MAKE) -C generated_code/libs clean

gen_libs_all : | gen_libs_clean
	- echo -e "$(OK_COLOR)Building gen libs object file(s)$(NO_COLOR)"
	$(MAKE) -C generated_code/libs build_obj
	- echo -e "$(OK_COLOR)Building gen libs static file(s)$(NO_COLOR)"
	$(MAKE) -C generated_code/libs build_lib
	- echo -e "$(OK_COLOR)Installing gen libs static file(s)$(NO_COLOR)"
	$(MAKE) -C generated_code/libs install

gen_libs_all_uts : | gen_libs_all
	- echo -e "$(OK_COLOR)Building gen libs unit test file(s)$(NO_COLOR)"
	$(MAKE) -C generated_code/libs build_uts

gen_libs_exchange : | gen_libs_clean
	- echo -e "$(OK_COLOR)Building gen libs object file(s)$(NO_COLOR)"
	$(MAKE) -C generated_code/libs build_obj
	- echo -e "$(OK_COLOR)Building gen libs static file(s)$(NO_COLOR)"
	$(MAKE) -C generated_code/libs build_lib
	- echo -e "$(OK_COLOR)Installing gen libs static file(s)$(NO_COLOR)"
	$(MAKE) -C generated_code/libs install

libs_clean :
	- echo -e "$(OK_COLOR)Cleaning libs$(NO_COLOR)"
	$(MAKE) -C libs clean

libs_all : | libs_clean gen_libs_all
	- echo -e "$(OK_COLOR)Building libs object file(s)$(NO_COLOR)"
	$(MAKE) -C libs build_obj
	- echo -e "$(OK_COLOR)Building libs static file(s)$(NO_COLOR)"
	$(MAKE) -C libs build_lib
	- echo -e "$(OK_COLOR)Installing libs static file(s)$(NO_COLOR)"
	$(MAKE) -C libs install

libs_all_uts : | libs_all
	- echo -e "$(OK_COLOR)Building libs unit test file(s)$(NO_COLOR)"
	$(MAKE) -C libs build_uts

libs_exchange : | libs_clean gen_libs_exchange
	- echo -e "$(OK_COLOR)Building libs object file(s)$(NO_COLOR)"
	$(MAKE) -C libs build_obj
	- echo -e "$(OK_COLOR)Building libs static file(s)$(NO_COLOR)"
	$(MAKE) -C libs build_lib
	- echo -e "$(OK_COLOR)Installing libs static file(s)$(NO_COLOR)"
	$(MAKE) -C libs install

$(SUBDIRS) :
	$(MAKE) new -C $@
	$(MAKE) install -C $@

$(PARALLEL_SUBDIRS) : | gen_libs_all_uts libs_all_uts
	- echo -e "$(OK_COLOR)Building $(@:parallel-%=%)$(NO_COLOR)"
	$(MAKE) new -C $(@:parallel-%=%)
	$(MAKE) install -C $(@:parallel-%=%)

$(BUILD_SUBDIRS) : 
	$(MAKE) new -C $(@:build-%=%)
	$(MAKE) install -C $(@:build-%=%)

$(COV_SUBDIRS) :
	$(MAKE) -C $(@:cov-%=%) coverage
	$(MAKE) install -C $(@:cov-%=%)

$(PROF_SUBDIRS) :
	$(MAKE) -C $(@:prof-%=%) profile
	$(MAKE) install -C $(@:prof-%=%)

$(CLEANUP_SUBDIRS) :
	$(MAKE) -C $(@:cleanup-%=%) clean
	rm -f *.junit.xml
	rm -f coverage.xml

$(EXCHANGE_SUBDIRS) : | libs_exchange
	- echo -e "$(OK_COLOR)Building $(@:exchange-%=%)$(NO_COLOR)"
	$(MAKE) -C $(@:exchange-%=%) clean
	$(MAKE) -C $(@:exchange-%=%) exchange
	$(MAKE) _install -C $(@:exchange-%=%)

$(INSTALL_SUBDIRS) : 
	$(MAKE) _install -C $(@:install-%=%)

$(UT_SUBDIRS) :
	$(MAKE) -C $(@:utest-%=%) run_ut

$(UT_COV_SUBDIRS) :
	$(MAKE) -C $(@:utest_cov-%=%) run_ut_cov

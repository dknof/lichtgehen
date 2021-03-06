DEPTH = .
DIRECTORY = .

include Makefile.modules

LIBS_ALL = 
ifeq ($(USE_UI_GTKMM), true)
  LIBS_ALL += `pkg-config gtkmm-3.0 --libs`
endif
ifeq ($(USE_THREADS), true)
  LIBS_ALL += -lpthread
endif

include Makefile.objects

.PHONY: all
all : lichtgehen

lichtgehen : objects directories
	$(CXX) $(CXXFLAGS) \
	  $(OBJECTS) \
	  $(DIRECTORIES:%=%/lib.a) \
	  $(LIBS_ALL) \
	  -o $@

# implementation

# Automatisch neu kompilieren, wenn sich eine Datei geändert hat
.PHONY: watch
watch :
	@$(RM) commands.sh
	@echo '#!/bin/bash' >> commands.sh
	@echo 'if [ -e ./lichtgehen ]; then' >> commands.sh
	@echo '  for f in $$(find . -name "*.cpp"); do' >> commands.sh
	@echo '    if [ $$f -nt ./lichtgehen ]; then rm ./lichtgehen; break; fi' >> commands.sh
	@echo '  done' >> commands.sh
	@echo 'fi' >> commands.sh
	@echo 'if [ $$(grep -c processor /proc/cpuinfo) -gt 1 ]; then' >> commands.sh
	@echo '  nice -n 5 make -s -k --jobs=$$(expr 1 + $$(grep -c processor /proc/cpuinfo) ) --load-average=$$(expr 1 + $$(grep -c processor /proc/cpuinfo) ) && sleep 1' >> commands.sh
	@echo 'else' >> commands.sh
	@echo '  nice -n 5 make -s -k && sleep 1' >> commands.sh
	@echo 'fi' >> commands.sh
	@while true; do \
	  find . -name '*.cpp' -o -name '*.h' -o -name 'Makefile*' | inotifywait -e modify --fromfile -; \
	  sh ./commands.sh; \
	  done
	@$(RM) commands.sh

include $(DEPTH)/Makefile.rules

.PHONY: tgz
tgz : clean
	$(RM) commands.sh
	cd ../..; \
	  $(RM) lichtgehen.tgz; \
	tar czvf lichtgehen.tgz lichtgehen --exclude .git --exclude .gitignore --exclude freiesmagazin-2014-10-contest --exclude lichtgehen.vimsession --exclude src/lichtgehen

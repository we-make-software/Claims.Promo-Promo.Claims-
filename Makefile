SHELL := /bin/bash
MAKEFLAGS += --no-print-directory

A28 := $(shell pwd)
A29 ?= /lib/modules/$(shell uname -r)/build

A2:= \033[0;31m
A3:= \033[0;32m
A4:= \033[1;33m
A5:= \033[0;36m
A6:= \033[0m

A1 := $(sort $(patsubst %/,%,$(wildcard */)))
A0 := ApplicationProgrammingInterface $(filter-out ApplicationProgrammingInterface LauncherInterface,$(A1)) LauncherInterface

A30:= $(foreach A28,$(A0),$(A28)/$(A28))
obj-m := $(addsuffix .o,$(A30))

all:
	@A7=0; \
	A8=0;  A9=1;  A10=2;  A11=3;  A12=4;  A13=5;  \
	A14="Test"; \
	A15="Log"; \
	A16="Clean"; \
	A17="Build"; \
	A18="Help"; \
	A19="Exit"; \
	A23=0; \
	for A31 in $(A0); do \
	if 	compgen -G "$$A31/*.ko" > /dev/null || \
   		compgen -G "$$A31/*.o" > /dev/null || \
   		compgen -G "$$A31/*.cmd" > /dev/null || \
   		compgen -G "$$A31/*.mod" > /dev/null; then \
    A23=1; \
	fi; \
	done; \
	[ $$A23 -eq 0 ] && A16=""; \
	while true; do \
	  clear; \
	  echo -e "$(A3)===================================================================\$(A6)"; \
	  echo -e "$(A4)Welcome to the Makefile Smart System made by Promo.Claims | Claims.Promo \$(A6)"; \
	  echo -e "$(A3)===================================================================\$(A6)"; \
	  echo; \
	  echo -e "$(A5)Build order:\$(A6)"; \
	  echo "  $(A0)"; \
	  echo; \
	  echo -e "$(A3)===================================================================\$(A6)"; \
	  echo; \
	  echo "Use arrow keys to move and Enter to select:"; \
	  for A20 in $$A8 $$A9 $$A10 $$A11 $$A12 $$A13; do \
	    case $$A20 in \
	      $$A8) A21=$$A14 ;; \
	      $$A9) A21=$$A15 ;; \
	      $$A10) A21=$$A16 ;; \
	      $$A11) A21=$$A17 ;; \
	      $$A12) A21=$$A18 ;; \
	      $$A13) A21=$$A19 ;; \
	    esac; \
	    if [ -n "$$A21" ]; then \
	      if [ $$A20 -eq $$A7 ]; then echo "[x] $$A21"; else echo "[ ] $$A21"; fi; \
	    fi; \
	  done; \
	  read -rsn1 A22; \
	  case "$$A22" in \
	    A) A7=$$((A7-1)); if [ $$A7 -lt $$A8 ]; then A7=$$A13; fi ;; \
	    B) A7=$$((A7+1)); if [ $$A7 -gt $$A13 ]; then A7=$$A8; fi ;; \
	    "") break ;; \
	  esac; \
	done; \
	case $$A7 in \
		$$A8) $(MAKE) A24 ;; \
		$$A9) $(MAKE) A25 ;; \
		$$A10) $(MAKE) A22 ;; \
		$$A11) $(MAKE) A23 ;; \
		$$A12) $(MAKE) A26 ;; \
	esac


A27:
	$(MAKE) -C $(A29) M=$(A28) clean
	@for A31 in $(shell echo $(A0) | awk '{for(i=NF;i>0;i--) print $$i}'); do \
		rmmod $$A31 2>/dev/null || echo "Module $$A31 not loaded, skipping"; \
		if compgen -G "$$A31/*.{ko,o,cmd,mod}" > /dev/null; then \
			rm -f $$A31/*.{ko,o,cmd,mod} 2>/dev/null || true; \
		fi; \
	done



A22:
	$(MAKE) A27 
	$(MAKE) all

A23:
	@A17="We build (Build)"; echo "$$A17";

A24:
	dmesg -C
	$(MAKE) A27 
	$(MAKE) -C $(A29) M=$(A28) modules
	@for A31 in $(A30); do \
	    if [ -f "$$A31.ko" ]; then \
	        insmod $$A31.ko 2>/dev/null || echo "Module $$A31.ko already loaded or failed"; \
	    fi; \
	done
	$(MAKE) A25 

A25:
	 dmesg -w

A26:
	@read -p "What do you want help with? " msg; \
	git add .; \
	git commit -m "$$msg"; \
	git push -u origin master


.PHONY: all A22 A23 A24 A25 A26

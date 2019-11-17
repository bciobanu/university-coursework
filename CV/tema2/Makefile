define run_all_policies
    python3 main.py load -i $(DATADIR)/$(1).jpg $(2) -p OPTIMAL save -o $(RESULTSDIR)/$(1)_optim.jpg
    python3 main.py load -i $(DATADIR)/$(1).jpg $(2) -p GREEDY save -o $(RESULTSDIR)/$(1)_greedy.jpg
    python3 main.py load -i $(DATADIR)/$(1).jpg $(2) -p RANDOM save -o $(RESULTSDIR)/$(1)_random.jpg
endef

RESULTSDIR = results
DATADIR = data

.PHONY: all clean

$(RESULTSDIR):
	mkdir -p $(RESULTSDIR)

$(RESULTSDIR)/castel.jpg: $(RESULTSDIR)
	python3 main.py load -i $(DATADIR)/castel.jpg resize -c -50 save -o $(RESULTSDIR)/castel.jpg

$(RESULTSDIR)/praga.jpg: $(RESULTSDIR)
	python3 main.py load -i $(DATADIR)/praga.jpg resize -l -100 save -o $(RESULTSDIR)/praga.jpg

$(RESULTSDIR)/delfin.jpg: $(RESULTSDIR)
	python3 main.py load -i $(DATADIR)/delfin.jpeg resize -l +50 -c +50 save -o $(RESULTSDIR)/delfin.jpg

$(RESULTSDIR)/arcTriumf.jpg: $(RESULTSDIR)
	python3 main.py load -i $(DATADIR)/arcTriumf.jpg resize -c -100 resize -c +100 save -o $(RESULTSDIR)/arcTriumf.jpg

$(RESULTSDIR)/lac.jpg: $(RESULTSDIR)
	python3 main.py load -i $(DATADIR)/lac.jpg cut save -o $(RESULTSDIR)/lac.jpg

$(RESULTSDIR)/pagoda_optim.jpg $(RESULTSDIR)/pagoda_greedy.jpg $(RESULTSDIR)/pagoda_random.jpg: $(RESULTSDIR)
	$(call run_all_policies,pagoda,resize -c +100)

$(RESULTSDIR)/edo_optim.jpg $(RESULTSDIR)/edo_greedy.jpg $(RESULTSDIR)/edo_random.jpg: $(RESULTSDIR)
	$(call run_all_policies,edo,resize -l +100 -c -100)

$(RESULTSDIR)/wines_optim.jpg $(RESULTSDIR)/wines_greedy.jpg $(RESULTSDIR)/wines_random.jpg: $(RESULTSDIR)
	$(call run_all_policies,wines,resize -c -50 resize -c +50)

$(RESULTSDIR)/crossing_optim.jpg $(RESULTSDIR)/crossing_greedy.jpg $(RESULTSDIR)/crossing_random.jpg: $(RESULTSDIR)
	$(call run_all_policies,crossing,resize -l -100)

$(RESULTSDIR)/oops_optim.jpg $(RESULTSDIR)/oops_greedy.jpg $(RESULTSDIR)/oops_random.jpg: $(RESULTSDIR)
	$(call run_all_policies,oops,resize -l +100)

all: $(RESULTSDIR)/castel.jpg $(RESULTSDIR)/praga.jpg $(RESULTSDIR)/delfin.jpg $(RESULTSDIR)/arcTriumf.jpg $(RESULTSDIR)/lac.jpg $(RESULTSDIR)/pagoda_optim.jpg $(RESULTSDIR)/edo_optim.jpg $(RESULTSDIR)/wines_optim.jpg $(RESULTSDIR)/crossing_optim.jpg $(RESULTSDIR)/oops_optim.jpg

clean:
	rm -rf $(RESULTSDIR)

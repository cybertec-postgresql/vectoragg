MODULES = vectoragg
EXTENSION = vectoragg
DATA = vectoragg--1.0.sql
DOCS = README.vectoragg

PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
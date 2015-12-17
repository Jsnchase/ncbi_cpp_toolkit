# $Id$

APP = tds95_dynamic1
SRC = dynamic1 common

CPPFLAGS = -DHAVE_CONFIG_H=1 $(FTDS95_INCLUDE) $(ORIG_CPPFLAGS)
LIB      = tds_ftds95$(STATIC)
LIBS     = $(FTDS95_CTLIB_LIBS) $(NETWORK_LIBS) $(RT_LIBS) $(C_LIBS)
LINK     = $(C_LINK)

CHECK_CMD  = test-tds95.sh tds95_dynamic1
CHECK_COPY = test-tds95.sh

CHECK_REQUIRES = in-house-resources

WATCHERS = ucko
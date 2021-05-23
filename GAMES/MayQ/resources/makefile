#! make -f
#
# makefile - start
#


# directory
#

# output directory
OUTDIR			=	..

# tool directory
TOOLDIR			=	tools

# build resource file
#
resource:
	@$(TOOLDIR)/chr2cpph -n fontTable -o $(OUTDIR)/font.cpp font.chr
	@$(TOOLDIR)/chr2cpph -n patternTable -o $(OUTDIR)/pattern.cpp pattern.chr

# build tools
#
tool:
	@gcc -o $(TOOLDIR)/chr2cpph $(TOOLDIR)/chr2cpph.cpp


# makefile - end

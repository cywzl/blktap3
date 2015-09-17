
# ACLOCAL_AMFLAGS = -I m4

# FIXME run cppcheck

SUBDIRS  = lvm
SUBDIRS += $(MAYBE_part)
SUBDIRS += vhd
SUBDIRS += control
SUBDIRS += drivers
SUBDIRS += include
SUBDIRS += tapback

if ENABLE_PART
MAYBE_part = part
endif

package:
	git archive --format tar --prefix blktap-3.0.0.xs992/ HEAD | bzip2 > blktap-3.0.0.xs992.tar.bz2
rpm: package
	mv blktap-3.0.0.xs992.tar.bz2 $(RPMROOT)/SOURCES
	cp redhat/blktap.spec $(RPMROOT)/SPECS
	rpmbuild --define="release ${rpmversion}" -ba $(RPMROOT)/SPECS/blktap.spec

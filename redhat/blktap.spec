Summary: blktap user space utilities
Name: blktap
Version: 3.0.0.xs992
Release: %release
License: GPLv2 or BSD
Group: System/Hypervisor
Source0: %{name}-%{version}.tar.bz2

#Patch1: %{name}-udev-ignore-tapdevs.patch
BuildRoot: %{_tmppath}/%{name}-%{release}-buildroot
Obsoletes: xen-blktap
BuildRequires: e2fsprogs-devel, libaio-devel
%description
Blktap creates kernel block devices which realize I/O requests to
processes implementing virtual hard disk images entirely in user
space.

Typical disk images may be implemented as files, in memory, or
stored on other hosts across the network. The image drivers included
with tapdisk can map disk I/O to sparse file images accessed through
Linux DIO/AIO and VHD images with snapshot functionality.

This packages includes the control utilities needed to create
destroy and manipulate devices ('tap-ctl'), the 'tapdisk' driver
program to perform tap devices I/O, and a number of image drivers.

%package devel
Summary: BlkTap Development Headers and Libraries
Requires: blktap = %{version}
Group: Development/Libraries
Obsoletes: xen-blktap

%description devel
Blktap and VHD development files.

%prep
%setup -q
#%patch1 -p1

%build
CFLAGS='-O0 -g -pipe -Wall -Wp,-D_FORTIFY_SOURCE=2 -fexceptions -fstack-protector --param=ssp-buffer-size=4 -m64 -mtune=generic'
CXXFLAGS='-O0 -g -pipe -Wall -Wp,-D_FORTIFY_SOURCE=2 -fexceptions -fstack-protector --param=ssp-buffer-size=4 -m64 -mtune=generic'
FFLAGS='-O0 -g -pipe -Wall -Wp,-D_FORTIFY_SOURCE=2 -fexceptions -fstack-protector --param=ssp-buffer-size=4 -m64 -mtune=generic'
%configure
make

%install
rm -rf %{buildroot}
make install DESTDIR=%{buildroot}

%files
%defattr(-,root,root,-)
%doc
%{_libdir}/*.so
%{_libdir}/*.so.*
%{_bindir}/vhd-util
%{_bindir}/vhd-update
%{_bindir}/vhd-index
%{_bindir}/tapback
%{_sbindir}/lvm-util
%{_sbindir}/tap-ctl
%{_sbindir}/td-util
%{_sbindir}/td-rated
%{_sbindir}/part-util
%{_sbindir}/vhdpartx
%{_libexecdir}/tapdisk
%{_sysconfdir}/udev/rules.d/blktap.rules
%{_sysconfdir}/rc.d/init.d/tapback
%{_sysconfdir}/logrotate.d/blktap
%{_sysconfdir}/xensource/bugtool/tapdisk-logs.xml
%{_sysconfdir}/xensource/bugtool/tapdisk-logs/description.xml

%files devel
%defattr(-,root,root,-)
%doc
%{_libdir}/*.a
%{_libdir}/*.la
%{_includedir}/vhd/*
%{_includedir}/blktap/*

%post
[ ! -x /sbin/chkconfig ] || chkconfig --add tapback

%changelog
* Wed Sep 17 2014 Citrix Systems, Inc. <www.citrix.com> [3.0.0.xs992-xs6.5.0]
- Build blktap.

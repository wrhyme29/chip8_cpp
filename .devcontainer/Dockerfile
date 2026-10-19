FROM registry.access.redhat.com/ubi9/ubi:9.6-1753978585
USER root
RUN dnf install -y --nogpgcheck \
      bash-completion-1:2.11-5.el9 \
      cmake-3.31.8-3.el9 \
      gcc-11.5.0-14.el9 \
      gcc-c++-11.5.0-14.el9 \
      gdb-16.3-3.1.el9_8 \
      git-2.52.0-1.el9 \
      ncurses-6.2-12.20210508.el9 \
      rpmdevtools-9.5-1.el9 \
      vim-common-2:8.2.2637-26.el9_8.23 && \
    dnf clean all
ENTRYPOINT ['/bin/bash']

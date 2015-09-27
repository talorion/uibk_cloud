TEMPLATE = subdirs

CONFIG += debug


SUBDIRS +=    qcustomplot
SUBDIRS +=    uibk_cloud


uibk_cloud.depends           += qcustomplot

# build must be last:
CONFIG += ordered


#ifndef __NVME_H_INCLUDED__
#define __NVME_H_INCLUDED__

#include <drivers/pci.h>
#include <hal/drivers/storage/nvme.h>
#include <hal/proc/isrhandler.h>

bool nvme_detect_from_pci_bus(struct pci_address addr,
							  struct hal_nvme_controller *buf);

#endif
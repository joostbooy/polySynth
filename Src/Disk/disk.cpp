#include "disk.h"

Sdio* Disk::sdio_;

DSTATUS disk_status (BYTE pdrv) {
	if ((pdrv >= 1) || (Disk::sdio_->status() == false)) {
		return STA_NOINIT;
	}
	return 0;
}

DSTATUS disk_initialize (BYTE pdrv) {
	if ((pdrv >= 1) || (Disk::sdio_->status() == false)) {
		return STA_NOINIT;
	}
	return 0;
}

DRESULT disk_read (BYTE pdrv, BYTE *buff, LBA_t sector, UINT count) {
	if (pdrv >= 1) {
		return RES_ERROR;
	}

	if (Disk::sdio_->read(buff, sector, count)) {
		return RES_OK;
	}

	return RES_ERROR;
}

#if FF_FS_READONLY == 0
DRESULT disk_write (BYTE pdrv, const BYTE *buff, LBA_t sector, UINT count) {
	if (pdrv >= 1) {
		return RES_ERROR;
	}

	if (Disk::sdio_->write(buff, sector, count)) {
		return RES_OK;
	}

	return RES_ERROR;
}
#endif

DRESULT disk_ioctl (BYTE pdrv, BYTE cmd, void *buff) {
	if (pdrv >= 1) {
		return RES_ERROR;
	}

	switch (cmd)
	{
	case CTRL_SYNC:
		return RES_OK;
	case GET_SECTOR_COUNT:
		*static_cast<DWORD*>(buff) = Disk::sdio_->sectorCount();
		return RES_OK;
	case GET_SECTOR_SIZE:
		*static_cast<WORD*>(buff) = 512;
		return RES_OK;
	case GET_BLOCK_SIZE:
		*static_cast<DWORD*>(buff) = 1;
		return RES_OK;
	default:
		break;
	}

	return RES_PARERR;
}

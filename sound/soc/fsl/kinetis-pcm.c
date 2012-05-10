/*
 * Copyright (C) 2010 Freescale Semiconductor, Inc. All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/dma-mapping.h>

#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>

#include <mach/hardware.h>

#include "kinetis-pcm.h"
static const struct snd_pcm_hardware kinetis_pcm_hardware = {
	.info			= SNDRV_PCM_INFO_MMAP |
				  SNDRV_PCM_INFO_MMAP_VALID |
				  SNDRV_PCM_INFO_PAUSE |
				  SNDRV_PCM_INFO_RESUME |
				  SNDRV_PCM_INFO_INTERLEAVED,
	.formats		= SNDRV_PCM_FMTBIT_S16_LE |
				  SNDRV_PCM_FMTBIT_S20_3LE |
				  SNDRV_PCM_FMTBIT_S24_LE |
				  SNDRV_PCM_FMTBIT_S32_LE,
	.channels_min		= 2,
	.channels_max		= 2,
	.period_bytes_min	= 32,
	.period_bytes_max	= 8192,
	.periods_min		= 1,
	.periods_max		= 255,
	.buffer_bytes_max	= 64 * 1024,
	.fifo_size		= 32,
};

/*
 * Required to request DMA channels
 */
struct device *mxs_pcm_dev;


static inline void *snd_soc_dai_get_dma_data(const struct snd_soc_dai *dai,
					     const struct snd_pcm_substream *ss)
{
	return NULL;
}

static inline void snd_soc_dai_set_dma_data(struct snd_soc_dai *dai,
					    const struct snd_pcm_substream *ss,
					    void *data)
{

}

/*
 * Make a circular DMA descriptor list
 */
static int mxs_pcm_prepare(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct k70_runtime_data *prtd = runtime->private_data;
	dma_addr_t dma_buffer_phys;
	int periods_num, playback, i;

	return 0;
}

/*
 * Stop circular DMA descriptor list
 * We should not stop DMA in a middle of current transaction once we receive
 * stop request from ALSA core. This function finds the next DMA descriptor
 * and set it up to decrement DMA channel semaphore. So the current transaction
 * is the last data transfer.
 */
static void mxs_pcm_stop(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct k70_runtime_data *prtd = runtime->private_data;
	int desc;

	/* Freez DMA channel for a moment */

}

static int mxs_pcm_trigger(struct snd_pcm_substream *substream, int cmd)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct k70_runtime_data *prtd = substream->runtime->private_data;
	int ret = 0;

	return ret;
}

static snd_pcm_uframes_t
mxs_pcm_pointer(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct k70_runtime_data *prtd = runtime->private_data;
	unsigned int offset;
	dma_addr_t pos;

	return offset;
}

static int mxs_pcm_hw_params(struct snd_pcm_substream *substream,
				  struct snd_pcm_hw_params *hw_params)
{
	struct k70_runtime_data *prtd = substream->runtime->private_data;

	prtd->format = params_format(hw_params);

	return snd_pcm_lib_malloc_pages(substream,
					params_buffer_bytes(hw_params));
}

static int mxs_pcm_hw_free(struct snd_pcm_substream *substream)
{
	return snd_pcm_lib_free_pages(substream);
}

static int mxs_pcm_open(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct k70_runtime_data *prtd;
	int ret;

	/* Ensure that buffer size is a multiple of the period size */
	ret = snd_pcm_hw_constraint_integer(runtime,
					SNDRV_PCM_HW_PARAM_PERIODS);
	if (ret < 0)
		return ret;

	snd_soc_set_runtime_hwparams(substream, &kinetis_pcm_hardware);

	prtd = kzalloc(sizeof(struct k70_runtime_data), GFP_KERNEL);
	if (prtd == NULL)
		return -ENOMEM;

	runtime->private_data = prtd;

	return 0;
}

static int mxs_pcm_close(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct k70_runtime_data *prtd = runtime->private_data;
	int desc_num = kinetis_pcm_hardware.periods_max;
	int desc;

	static LIST_HEAD(list);

	/* Free private runtime data */
	kfree(prtd);
	return 0;
}

static int mcs_pcm_copy(struct snd_pcm_substream *substream, int channel,
			snd_pcm_uframes_t hwoff, void __user *buf,
			snd_pcm_uframes_t frames)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct k70_runtime_data *prtd = runtime->private_data;
	char *hwbuf = runtime->dma_area + frames_to_bytes(runtime, hwoff);
	unsigned long count = frames_to_bytes(runtime, frames);

	return 0;
}

static int mxs_pcm_mmap(struct snd_pcm_substream *substream,
			     struct vm_area_struct *vma)
{
	struct snd_pcm_runtime *runtime = substream->runtime;

}

struct snd_pcm_ops kinetis_pcm_ops = {
	.open		= mxs_pcm_open,
	.close		= mxs_pcm_close,
	.ioctl		= snd_pcm_lib_ioctl,
	.hw_params	= mxs_pcm_hw_params,
	.hw_free	= mxs_pcm_hw_free,
	.prepare	= mxs_pcm_prepare,
	.trigger	= mxs_pcm_trigger,
	.pointer	= mxs_pcm_pointer,
	.copy		= mcs_pcm_copy,
	.mmap		= mxs_pcm_mmap,
};

static u64 mxs_pcm_dma_mask = DMA_BIT_MASK(32);

static int kinetis_pcm_new(struct snd_card *card,
			    struct snd_soc_dai *dai, struct snd_pcm *pcm)
{
	size_t size = kinetis_pcm_hardware.buffer_bytes_max;

	if (!card->dev->dma_mask)
		card->dev->dma_mask = &mxs_pcm_dma_mask;

	if (!card->dev->coherent_dma_mask)
		card->dev->coherent_dma_mask = DMA_BIT_MASK(32);

	snd_pcm_lib_preallocate_pages_for_all(pcm, SNDRV_DMA_TYPE_DEV, NULL,
					      size, size);

	return 0;
}

static void kinetis_pcm_free(struct snd_pcm *pcm)
{
	snd_pcm_lib_preallocate_free_for_all(pcm);
}

/*
 * We need probe/remove callbacks to setup mxs_pcm_dev
 */
static int kinetis_pcm_probe(struct platform_device *pdev)
{
	mxs_pcm_dev = &pdev->dev;
	return 0;
}

static int kinetis_pcm_remove(struct platform_device *pdev)
{
	mxs_pcm_dev = NULL;
	return 0;
}

struct snd_soc_platform kinetis_soc_platform = {
	.name		= "Kinetis Audio",
	.pcm_ops	= &kinetis_pcm_ops,
	.probe		= kinetis_pcm_probe,
	.remove		= kinetis_pcm_remove,
	.pcm_new	= kinetis_pcm_new,
	.pcm_free	= kinetis_pcm_free,
};
EXPORT_SYMBOL_GPL(kinetis_soc_platform);

static int __init kinetis_pcm_init(void)
{
	return snd_soc_register_platform(&kinetis_soc_platform);
}

static void __exit kinetis_pcm_exit(void)
{
	snd_soc_unregister_platform(&kinetis_soc_platform);
}
module_init(kinetis_pcm_init);
module_exit(kinetis_pcm_exit);

MODULE_AUTHOR("Freescale Semiconductor, Inc.");
MODULE_DESCRIPTION("Kinetis SAI Module");
MODULE_LICENSE("GPL");

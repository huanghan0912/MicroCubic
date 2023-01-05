#include "Sd.h"


static const char *TAG = "Sd";

/**
 * @brief 初始化sd配置
 * 
 */

void Sd::Init(){
    // VFS配置
    //挂载文件系统的选项。
    //如果 format_if_mount_failed 设置为 true，SD 卡将被分区并格式化以防挂载失败。
	mount_config.format_if_mount_failed = false;
    mount_config.max_files = 5; //同时最大打开文件数
    mount_config.allocation_unit_size = 16 * 1024;
    

	ESP_LOGI(TAG, "Initializing SD card");

	// 初始化SPI总线
    
    //host配置
	host.flags = SDMMC_HOST_FLAG_SPI | SDMMC_HOST_FLAG_DEINIT_ARG;
    host.slot = SDSPI_DEFAULT_HOST;
    host.max_freq_khz = SDMMC_FREQ_DEFAULT;
    host.io_voltage = 3.3f;
    host.init = &sdspi_host_init;
    host.set_bus_width = NULL;
    host.get_bus_width = NULL;
    host.set_bus_ddr_mode = NULL;
    host.set_card_clk = &sdspi_host_set_card_clk;
    host.do_transaction = &sdspi_host_do_transaction;
    host.deinit_p = &sdspi_host_remove_device;
    host.io_int_enable = &sdspi_host_io_int_enable;
    host.io_int_wait = &sdspi_host_io_int_wait;
    host.command_timeout_ms = 0;

    //spi配置
	bus_cfg.mosi_io_num = SD_MOSI;
	bus_cfg.miso_io_num =SD_MISO;
	bus_cfg.sclk_io_num = SD_CLK;
	bus_cfg.quadwp_io_num = -1;
	bus_cfg.quadhd_io_num = -1;
	bus_cfg.max_transfer_sz = 4000;


   // 初始化SPI总线
    esp_err_t  ret = spi_bus_initialize((spi_host_device_t)host.slot, &bus_cfg, SPI_DMA_CHAN);
	if (ret != ESP_OK) {
		ESP_LOGE(TAG, "Failed to initialize bus.");
		return;
	}
    slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
    slot_config.gpio_cs = SD_CS;
    slot_config.host_id =(spi_host_device_t)host.slot;

    ESP_LOGI(TAG, "Mounting filesystem");//挂载文件系统
    //一体化功能
    //据 slot_config 中配置的 SPI Master 驱动程序初始化 SPI Master 设备，并将其连接到已初始化的 SPI 总线。
    //使用 host_config_input 中的配置初始化 SD 卡
    //使用 FATFS 库在 SD 卡上挂载 FAT 分区，在 mount_config 中进行配置
    //向 VFS 注册 FATFS 库，前缀由 base_prefix 变量给出
    ret = esp_vfs_fat_sdspi_mount(mount_point, &host, &slot_config, &mount_config, &card);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount filesystem. "
                     "If you want the card to be formatted, set the EXAMPLE_FORMAT_IF_MOUNT_FAILED menuconfig option.");
        } else {
            ESP_LOGE(TAG, "Failed to initialize the card (%s). "
                     "Make sure SD card lines have pull-up resistors in place.", esp_err_to_name(ret));
        }
        return;
    }
    ESP_LOGI(TAG, "Filesystem mount finish");

    // 输出卡的信息
    sdmmc_card_print_info(stdout, card);

}
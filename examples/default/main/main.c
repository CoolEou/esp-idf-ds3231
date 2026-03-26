#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <ds3231.h>
#include <string.h>

void ds3231_test(void *pvParameters)
{
    i2c_dev_t dev;
    memset(&dev, 0, sizeof(i2c_dev_t));

    ESP_ERROR_CHECK(ds3231_init_desc(&dev, 0, CONFIG_EXAMPLE_I2C_MASTER_SDA, CONFIG_EXAMPLE_I2C_MASTER_SCL));

    // setup datetime: 2016-10-09 13:50:10
    struct tm time =
    {
        .tm_year = 116, //since 1900 (2016 - 1900)
        .tm_mon  = 9,  // 0-based
        .tm_mday = 9,
        .tm_hour = 13,
        .tm_min  = 50,
        .tm_sec  = 10
    };
    ESP_ERROR_CHECK(ds3231_set_time(&dev, &time));

    // setup alarmtime: friday at 17:25
    struct tm alarm =
    {
        .tm_wday = 4,
        .tm_hour = 17,
        .tm_min  = 25,
    };
    ESP_ERROR_CHECK(ds3231_set_alarm2(&dev, &alarm, DS3231_ALARM2_MATCH_SECMINHOURDAY));

    // setup alarmtime: 9th day of month at 13:51:10
    alarm =
    {
        .tm_mday = 9,
        .tm_hour = 13,
        .tm_min  = 51,
        .tm_sec  = 10
    };
    ESP_ERROR_CHECK(ds3231_set_alarm1(&dev, &alarm, DS3231_ALARM1_MATCH_SECMINHOURDATE));

    if (ds3231_get_alarm2(&dev, &alarm) != ESP_OK)
    {
        printf("Could not get alarm2\n");
        continue;
    }
    printf("Alarm 2 is set to: %02d day of week %02d:%02d", alarm.tm_wday, alarm.tm_hour, alarm.tm_min)

    if (ds3231_get_alarm1(&dev, &alarm) != ESP_OK)
    {
        printf("Could not get alarm1\n");
        continue;
    }
    printf("Alarm 1 is set to: %02d day of month %02d:%02d:%02d", time.tm_mon + 1, alarm.tm_mday, alarm.tm_hour, alarm.tm_min, time.tm_sec)

    // Set alarm int
    ds3231_enable_alarm_ints(&dev, DS3231_ALARM_1);

    // Check alarm int
    ds3231_alarm_t alarms = DS3231_ALARM_NONE;
    ds3231_get_alarm_ints(&dev, &alarms);

    printf("alarms active: %02d", alarms)

    while (1)
    {
        float temp;

        vTaskDelay(pdMS_TO_TICKS(250));

        if (ds3231_get_temp_float(&dev, &temp) != ESP_OK)
        {
            printf("Could not get temperature\n");
            continue;
        }

        if (ds3231_get_time(&dev, &time) != ESP_OK)
        {
            printf("Could not get time\n");
            continue;
        }

        ds3231_get_alarms_fired(&dev, &alarms);

        /* float is used in printf(). you need non-default configuration in
         * sdkconfig for ESP8266, which is enabled by default for this
         * example. see sdkconfig.defaults.esp8266
         */
        printf("%04d-%02d-%02d %02d:%02d:%02d, %.2f deg Cel, alarms fired: %02d\n", time.tm_year + 1900 /*Add 1900 for better readability*/, time.tm_mon + 1,
               time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec, temp, alarms);
    }
}

void app_main()
{
    ESP_ERROR_CHECK(i2cdev_init());
    xTaskCreate(ds3231_test, "ds3231_test", configMINIMAL_STACK_SIZE * 3, NULL, 5, NULL);
}


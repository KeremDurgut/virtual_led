#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/leds.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kerem Durgut");
MODULE_DESCRIPTION("Virtual LED Driver");

// Userspace /sys/class/leds/virtual::test_led/brightness dosyasına bir şey yazılırsa bu çalışçak
static int virtual_led_brightness_set(struct led_classdev *led_cdev, enum led_brightness value)
{
    pr_info("Virtual LED parlakligi: %d\n", value);
    return 0; // Islemin basarili oldugunu belirtir
}

// LED subsystemi driverlardan sadece LED nesnesi ve belirli durumların handellanmasını istiyor. Bu LED nesnesi de aşağıda
// bu aşşadaki tanımlama yöntemi, aslında orijinalinden ÇOK daha kısa. Sanırım burada sadece umrumuzda olan kısımları tanımlıyoruz
// .name ve .max_brightness diye başında . olarak tanımlama C99'un ayarladığı bir yöntemmiş. Ayrıca GCC'nin daha az compatible olan yöntemi var . yok onda
static struct led_classdev virtual_led = {
    .name = "virtual::test_led",
    .max_brightness = 255, // Tırnaklar kaldırıldı
    .brightness_set_blocking = virtual_led_brightness_set,
};

static int __init keremgul_fonksiyon_init(void)
{
    int ret;

    ret = led_classdev_register(NULL, &virtual_led);
    if (ret < 0) {
        pr_err("Sanal LED init alamadi. Hata kodu: %d\n", ret);
        return ret;
    }

    pr_info("Sanal LED yuklendi\n");
    return 0;
}

static void __exit keremgulexit(void)
{
    pr_info("Sanal LED kapaniyor\n");
    led_classdev_unregister(&virtual_led);
}

module_init(keremgul_fonksiyon_init);
module_exit(keremgulexit);

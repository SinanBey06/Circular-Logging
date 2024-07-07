// Gömülü Yazılım Projesi 07.07.2024 (Last Updade: 16:30)
// Proje Adı : Circular Logging 
//******************************************************** 
// Sinan İlbey ve Doğukan Avcı Tarafından PİTON Şirketi için Hazırlanmıştır.
//********************************************************
#include <iostream> //Girdi/çıktı işlemleri için kütüphane
#include <fstream>  //dosya girdi çıktı işlemleri 
#include <string>   //sting işlemleri için kütüphane
#include <chrono>   //Zaman işlemleri için kütüphane    
#include <ctime>    //Zaman ve tarih işlemleri için
#include <thread>   //treah işlemleri için kütüphane
#include <cstdio>   // C standart girdi/çıktı işlemleri kütüphanesi  
#include <filesystem> //dosya yolu için kütüphane
#include <system_error>   // Sistem hataları ve istisnaları için
#include <queue>    //Kuyruk (FIFO) veri yapısı için kütüphane




struct input { 

// kullanıcıdan logging yapmak için dosya sayısı ve sıklığını input olarak aldık. (sıklık kaç dosya eklenirken kaç dosya silineceğini belirtir.)

    int numberFiles; //dosya sayısı
    int frequency; // sıklık
   
};

std::string getCurrentDateTime() {
    
    // Şu anki zaman bilgisini alma
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);

    // time_t türünü tm türüne dönüştür
    std::tm* timeinfo = std::localtime(&time_t_now);

    // Tarihi ve saat/dakika/saniye bilgisini string olarak formatla
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d_%H-%M-%S", timeinfo);

    return std::string(buffer);
}
using namespace std; // dosya kaldırmak için

int main() {


    std::queue<string> myQueue; // kuyruk yapılarını kullandık. ilk     giren ilk çıkar.
    
    input input;
// kaç dosya loglanacak
    std::cout << "Kaç dosya loglansın?: ";
    std::cin >> input.numberFiles;

    std::cout << "Hangi sıklıkla loglama yapılsın: "; //kaçtane            eklenip kaç tane silinecek.
    std::cin >> input.frequency;

    // burada kendi veri tabanımızı oluşturduk. hali hazırda dosya oluşturularbilir. Bu dosyalarda ekleme çıkarma yapılmaz.

    std::cout << "Girdiginiz değerler: " << input.numberFiles << "\t" << input.frequency << std::endl;

     for (int i = 1; i <= input.numberFiles; ++i) {

    // Şu anki tarih ve saat/dakika/saniye bilgisini al
    std::string tarihSaat = getCurrentDateTime();

    // Dosya adını oluştur: log_YYYY-MM-DD_HH-MM-SS.txt
    std::string dosyaAdi = "log_" + tarihSaat + ".txt";
          myQueue.push(dosyaAdi);
    // Dosyayı aç ve yazma modunda oluştur
    std::ofstream dosya(dosyaAdi);

    // Dosyanın açılıp açılmadığını kontrol et
    if (dosya.is_open()) {
        dosya << "Bu " << tarihSaat << " tarihli log dosyasidir.\n";
        dosya << "Buraya log mesajlarini yazabilirsiniz.\n";
        dosya.close(); // Dosyayı kapat
        std::cout << "Log dosyasi olusturuldu: " << dosyaAdi << std::endl;
    } else {
        std::cerr << "Hata: Dosya acilamadi!\n"; // Hata mesajı
    }
        //3 saniye bekleme
         std::this_thread::sleep_for(std::chrono::seconds(3));
     }



    std::cout << "***********************************" << std::endl;
   
    // Loglama işlemi
    
     for (int j = 1; j <= input.frequency; ++j)
     {

         // Şu anki tarih ve saat/dakika/saniye bilgisini al
         std::string tarihSaat = getCurrentDateTime();

         // Dosya adını oluştur: log_YYYY-MM-DD_HH-MM-SS.txt
         std::string dosyaAdi = "log_" + tarihSaat + ".txt";

         //3 saniye bekleme
          std::this_thread::sleep_for(std::chrono::seconds(3));
        
    myQueue.push(dosyaAdi); // Yeni dosya adını kuyruğa ekle
    // Yeni bir dosya oluştur ve hemen kapat
         ofstream dosyaYaz(dosyaAdi);
           dosyaYaz.close();
     // Kuyruğun en önündeki dosya adını al ve kuyruktan çıkar
     std::string silinecekDosya = myQueue.front();
     myQueue.pop();
     // Dosyayı sil
     std::remove(silinecekDosya.c_str());

     }
    // Kuyruğun tüm elemanlarını ekrana yaz
    std::cout << "\nKuyruğun tüm elemanları: ";
    while (!myQueue.empty()) {
        std::cout << myQueue.front() << "\n ";
        myQueue.pop(); // Kuyruğun önünden elemanları çıkar
    }
    std::cout << std::endl;
    
  
    return 0; // Programı sonlandır
}






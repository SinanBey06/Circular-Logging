// Gömülü Yazılım Projesi 07.07.2024 (Last Updade: 16:30)
// Proje Adı : Circular Logging
//********************************************************
// Sinan İlbey ve Doğukan Avcı Tarafından PİTON Şirketi için Hazırlanmıştır.
//********************************************************
// Kütüphane Tanımlama
#include "json.hpp"     // Kütüphane dosyasını proje dizininden dahil etme
#include <chrono>       //Zaman işlemleri için kütüphane
#include <cstdio>       // C standart girdi/çıktı işlemleri kütüphanesi
#include <ctime>        //Zaman ve tarih işlemleri için
#include <filesystem>   //dosya yolu için kütüphane
#include <fstream>      //dosya girdi çıktı işlemleri
#include <iostream>     //Girdi/çıktı işlemleri için kütüphane
#include <queue>        //Kuyruk (FIFO) veri yapısı için kütüphane
#include <string>       //sting işlemleri için kütüphane
#include <system_error> // Sistem hataları ve istisnaları için
#include <thread>       //treah işlemleri için kütüphane
//********************************************************
//********************************************************
struct input {

  // kullanıcıdan logging yapmak için dosya sayısı ve sıklığını input olarak
  // aldık. (sıklık kaç dosya eklenirken kaç dosya silineceğini belirtir.)

  int numberFiles; // dosya sayısı
  int frequency;   // sıklık
};
//********************************************************
//********************************************************
// Bilgisayardan zaman bilgisi çekme
std::string getCurrentDateTime() {

  // Şu anki zaman bilgisini alma
  auto now = std::chrono::system_clock::now();
  auto time_t_now = std::chrono::system_clock::to_time_t(now);

  // time_t türünü tm türüne dönüştür
  std::tm *timeinfo = std::localtime(&time_t_now);

  // Tarihi ve saat/dakika/saniye bilgisini string olarak formatla
  char buffer[80];
  std::strftime(buffer, sizeof(buffer), "%Y-%m-%d_%H-%M-%S", timeinfo);

  return std::string(buffer);
}
using namespace std; // dosya kaldırmak için
using json = nlohmann::json;
//********************************************************
//********************************************************
// Ana fonksiyon
int main() {

  //********************************************************
  //********************************************************
  // Logging türü seçimi
  int secenek;

  std::cout << "***********************************" << std::endl;
  std::cout << "***********************************\n" << std::endl;
  std::cout << " 1-Günlük Logging\n" << std::endl;
  std::cout << " 2-Saatlik Logging\n" << std::endl;
  std::cout << " 3-Dakikalık Logging\n" << std::endl;
  std::cout << " 4-Saniyelik Logging\n" << std::endl;
  std::cout << "***********************************" << std::endl;
  std::cout << "***********************************\n" << std::endl;
  std::cout << "Bir sayı girin (1-4 arası):\t";
  std::cin >> secenek;

  int zaman;
  int delay;
  switch (secenek) {

  case 1: // Günlük loglama seçeneği case 1 olarak atandı.
    std::cout << "\n***********************************\n" << std::endl;
    std::cout << "GÜNLÜK" << std::endl;
    std::cout << "***********************************\n" << std::endl;
    std::cout << "KAÇ GÜN SIKLIKLA LOGLAMA YAPILSIN" << std::endl;
    std::cin >> zaman;     // Loglama süresi girilmesi
    delay = zaman * 86400; // Bir gün 86400 Saniye aritmetik işlemi yapıldı.
    break;

  case 2: // Saatlik loglama seçeneği case 2 olarak atandı.
    std::cout << "\n***********************************" << std::endl;
    std::cout << "SAATLİK" << std::endl;
    std::cout << "***********************************\n" << std::endl;
    std::cout << "KAÇ SAATLİK SIKLIKLA LOGLAMA YAPILSIN" << std::endl;
    std::cin >> zaman;    // Loglama süresi girilmesi
    delay = zaman * 3600; // Bir saat 3600 Saniye aritmetik işlemi yapıldı.
    break;

  case 3: // Dakikalık loglama seçeneği case 3 olarak atandı.
    std::cout << "\n***********************************" << std::endl;
    std::cout << "DAKİKALIK" << std::endl;
    std::cout << "***********************************\n" << std::endl;
    std::cout << "KAÇ DAKİKALIK SIKLIKLA LOGLAMA YAPILSIN\t" << std::endl;
    std::cin >> zaman;  // Loglama süresi girilmesi
    delay = zaman * 60; // Bir dakika 60 Saniye aritmetik işlemi yapıldı.
    break;

  case 4: // Saniyelik loglama seçeneği case 4 olarak atandı.
    std::cout << "\n***********************************" << std::endl;
    std::cout << "SANİYELİK" << std::endl;
    std::cout << "***********************************\n" << std::endl;
    std::cout << "KAÇ SANİYELİK SIKLIKLA LOGLAMA YAPILSIN" << std::endl;
    std::cin >> zaman; // Loglama süresi girilmesi
    delay = zaman * 1;
    break;

  default:
    std::cout << "Geçersiz Seçenek." << std::endl;
    break;
  }
  //********************************************************
  //********************************************************
  // tanımlar

  std::queue<string>
      myQueue; // kuyruk yapılarını kullandık. ilk giren ilk çıkar.

  input input;
  // kaç dosya loglanacak
  std::cout << "***********************************\n" << std::endl;
  std::cout << "Kaç dosya loglansın?: ";
  std::cin >> input.numberFiles;

  std::cout << "Hangi sıklıkla loglama yapılsın: "; // kaçtane eklenip kaç tane
                                                    // silinecek.
  std::cin >> input.frequency;

  std::cout << "***********************************\n" << std::endl;

  //********************************************************
  //********************************************************

  // burada kendi veri tabanımızı oluşturduk. hali hazırda dosya
  // oluşturularbilir. Bu dosyalarda ekleme çıkarma yapılmaz.
  std::cout << "***********************************" << std::endl;
  std::cout << "Girdiginiz değerler: " << input.numberFiles << "\t"
            << input.frequency << std::endl;

  std::cout << "***********************************\n" << std::endl;
  std::cout << "Data base:" << std::endl;
  for (int i = 1; i <= input.numberFiles; ++i) {

    // 3 saniye bekleme
    std::this_thread::sleep_for(std::chrono::seconds(3));

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
  }

  std::cout << "***********************************" << std::endl;

  //********************************************************
  //********************************************************
  //********************************************************
  // Loglama işlemi

  for (int j = 1; j <= input.frequency; ++j) {
    // Loglama süresi seçimi
    std::this_thread::sleep_for(std::chrono::seconds(delay));

    // Şu anki tarih ve saat/dakika/saniye bilgisini al
    std::string tarihSaat = getCurrentDateTime();

    // Dosya adını oluştur: log_YYYY-MM-DD_HH-MM-SS.txt
    std::string dosyaAdi = "log_" + tarihSaat + ".txt";

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
  std::cout << "\nLogging sonrası dosyalar:\n ";
  while (!myQueue.empty()) {
    std::cout << myQueue.front() << "\n ";
    myQueue.pop(); // Kuyruğun önünden elemanları çıkar
  }
  std::cout << std::endl;

  //********************************************************
  //********************************************************
  // json formatına dönüştürme
  
  std::cout << "***********************************" << std::endl;
  std::cout << "\njson Formatı\n ";

  std::string str_delay = std::to_string(delay);
  json j;
  if (secenek == 1) {
    j["Dosya Loglama Türü"] = str_delay + " Günlük";
  } else if (secenek == 2) {
    j["Dosya Loglama Türü"] = str_delay + " Saatlik";
  } else if (secenek == 3) {
    j["Dosya Loglama Türü"] = str_delay + " Dakikalık";
  } else if (secenek == 4) {
    j["Dosya Loglama Türü"] = str_delay + " Saniyelik";
  }
  // JSON objesine veriler ekleme

  j["Loglanan Dosya Sayısı"] = input.numberFiles;
  j["Dosya loglama Frekansı (Sıklık)"] = input.frequency;

  // JSON objesini string olarak yazdırma
  std::cout
      << j.dump(4)
      << std::endl; // 4, JSON'un girinti (indentation) seviyesini belirtir.

  // JSON objesini dosyaya yazma
  std::ofstream file("Log_File.json");
  if (file.is_open()) {
    file << j.dump(4); // 4, JSON'un girinti seviyesini belirtir.
    file.close();
    std::cout << "JSON dosyası başarıyla oluşturuldu." << std::endl;
  } else {
    std::cerr << "Dosya açılamadı!" << std::endl;
  }
  //********************************************************
  //********************************************************
  //********************************************************

  return 0; // Programı sonlandır
}

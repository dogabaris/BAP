Nextion Rx --> digital(pwm) PIN13 Uno 
        Tx --> digital(pwm) PIN12 Uno 
        gprsSerial kartın dijital pinlerini seri dinlemek için kullanılır.
        sht11 sıcaklık nem sensörüdür. 
        HMISerial ekrandır
        Nextion 5" ekran dokunmatik bulunmaktadır.
        screen.HMI nextion idesinin ekran için yazılan koddur. Nextion Ideden bakılabilir.
        Ekrana sdkart ile ttf formatında tasarım aktarılır.
        Dijital pinler seri portmuş gibi çalışıtırlımak zorundadır çünkü rx0 tx0 bilgisayardan kod aktarılırken veya seri port dinlenirken,
        diğer seri port dinleyenlerle karışabilmektedir.
Arduino libraryleri conf klasöründedir
Kütüphanelerin
C://users/kişi adı/belgeler/arduino/library klasörüne atılması gerekir.
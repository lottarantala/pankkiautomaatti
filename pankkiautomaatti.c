#include <stdio.h>
#include <string.h>
void tyhjennaPuskuri(void);
void liittyman_lataus(double *raha);
void tarkasta_saldo(double *raha);
void valikko(double raha);
void otto(double *saldo);
void poistu(void);

//max nostovara on 1000e pankkiautomaateissa
#define max_nosto 1000.00

int main(void){
    int vertaus, saldo=0;
    char syote[256];
    char tilinumero[256];
    char tunnusluku[256];
    FILE *pankkitili;

    printf("Tervetuloa!\n");
    //pyydetaan tilinumero
    printf("Anna tilinumero, ole hyva\n");
    fgets(tilinumero, 256, stdin);
    //muutetaan rivinvaihto /0
    if(tilinumero[strlen(tilinumero)-1] == '\n')
        tilinumero[strlen(tilinumero)-1] = '\0';
    //muut vain tyhjataan
    else
        tyhjennaPuskuri();
    //lisataan tilinumeron peraan .acc jotta voidaan avata tiedosto
    strcat(tilinumero, ".acc");
    //jos tiedosto avataan onnistuneesti
    if((pankkitili = fopen(tilinumero, "r")) != NULL){
        printf("Nappaile tunnusluku ja paina enter\n");
        fgets(syote, 256, stdin);
        //vaihdetaan taas oikea paate
        if(syote[strlen(syote) - 1] == '\n')
            syote[strlen(syote) - 1] = '\0';
        //luetaan tunnusluku tiedostosta
        fgets(tunnusluku, 256, pankkitili);
        if(tunnusluku[strlen(tunnusluku) - 1] == '\n')
            tunnusluku[strlen(tunnusluku) - 1] = '\0';
            //teen tamankin koska minulla on windows
        if(tunnusluku[strlen(tunnusluku) - 1] == '\r')
            tunnusluku[strlen(tunnusluku) - 1] = '\0';
        //vertaus
        if((vertaus = strcmp(syote, tunnusluku)) == 0){
            //annetaan tietaa oikeasta pin-koodista
            printf("Tunnusluku oikein!\n");
            //luetaan saldo
            fscanf(pankkitili, "%d", &saldo);
            //suljetaan tilitiedosto
            fclose(pankkitili);
            //avataan valikko
            valikko((double)saldo);
        }
        else
            printf("\nVirheellinen tunnusluku\n");
    }
    else{
        printf("\nPankkitilia ei loydy\n");
    }
    return 0;
}

void valikko(double raha){
    int liittyma, toiminto, sisalto;
    double saldo = raha;
    //luodaan paavalikko joka pysyy valikossa niin kauan että valittu joku vaihtoehdoista
    do{
        printf("Voit valita seuraavista vaihtoehdoista:\n");
        printf("(1) Liittyman lataus puheaika\n");
        printf("(2) Otto\n");
        printf("(3) Saldo \n");
        printf("(4) Tapahtumat\n");
        printf("(5) Poistu\n");
        if((sisalto = (scanf("%d", &toiminto))) == 0)
            tyhjennaPuskuri();
    }while(toiminto > 5 || toiminto < 1);
    //kaytetaan switch valikkoa, jos valinta ei 1-5, invalidi valinta
    switch (toiminto){
        case 1:
            printf("Valittiin liittyman lataus puheaika\n");
            printf("Valitse liittyma:\n");
            printf("(1) Saunalahti\n");
            printf("(2) Dna\n");
            printf("(3) Go Mobile \n");
            printf("(4) Takaisin\n");
            scanf("%d", &liittyma);
            switch (liittyma){
            //liittymalla ei ole valia, joka tapauksessa laitetaan puh. num.
                case 1: case 2: case 3:
                    liittyman_lataus(&saldo);
                    break;
                case 4:
                    printf("Takaisin alkuun\n");
                    valikko(saldo);
                    break;
                default:
                    poistu();
                    break;
            }
            break;
        case 2:
            printf("Valittiin otto\n");
            otto(&saldo);
            break;
        case 3:
            printf("Valittiin saldo\n");
            tarkasta_saldo(&saldo);
            break;
        case 4:
            printf("Valittiin tapahtumat\n");
            printf("\nValitse:\n(1) takaisin | (muu) poistu\n");
            int tapahtuma_toiminto;
            scanf("%d", &tapahtuma_toiminto);
            switch (tapahtuma_toiminto){
                case 1:
                    valikko(saldo);
                        break;
                default:
                    poistu();
                    break;
            }
            break;
        case 5:
            poistu();
            break;
    }
}

void tyhjennaPuskuri(void){
    while(getchar() != '\n');
}

void liittyman_lataus(double *raha){
    //puhelinnumerossa on +358 ja 9 numeroa
    int numero, summa;
    double saldo;
    saldo = *raha;
    
    printf("Liittyman lataus\n");
    //laitetaan puhelinnumero
    printf("Syota puhelinnumero \n");
    printf("+358 ");
    scanf("%d",&numero);
    if(numero <= 99999999 || numero >= 1000000000){
        do{
            tyhjennaPuskuri();
            printf("Puhelinnumerosi ei ole oikean pituinen\n");
            //koska en osaa tarkistaa numeron pituutta jos ensimmäinen nro on 0,
            //asia korjattu laittamalla +358 ja 9 numeroa, siten ensimmäinen numero ei ole 0
            printf("+358 ");
            scanf("%d", &numero);
        }while(numero <= 99999999 || numero >= 1000000000);
    }
    //kun puhelinnumero validi, syotetaan ladattava summa
    //vaihtoehdot kopioitu simulaattorista
    printf("Valitse summa (e): 10, 15, 20, 25, 30, 50, 100\n");
    printf("Tai valitse (1) palataksesi takaisin alkuun\n");
    scanf("%d", &summa);
    switch (summa){
    case 10: case 15: case 20: case 25: case 30: case 50: case 100:
        //jos tarpeeksi saldoa, vahennetaan ladattu summa saldosta
        if(saldo > summa){
            saldo -= summa;
            //tulostetaan tiedot
            printf("Puhelinnumero: 0%d\n", numero);
            printf("Ladattava summa: %d+\n", summa);
            printf("Otto tililta: %d-\n", summa);
            //ilmoitetaan suoritetusta latauksesta
            printf("\nLataus suoritettu, latausvahvistus lahetetty tekstiviestilla\n");
            poistu();
        }
        //muuten palataan paavalikkoon
        else{
            printf("Tilillasi ei ole tarpeeksi saldoa, palataan takaisin paavalikkoon\n\n");
            valikko(saldo);
            break;
        }
        break;
    case 1:
        valikko(saldo);
        break;
    default:
        printf("Liittyman latausta ei voitu suorittaa, palataan alkuun\n");
        liittyman_lataus(&saldo);
        break;
    }
}

void tarkasta_saldo(double *raha){
    int toiminto;
    double saldo = *raha;
    double kayttoraja = saldo;

    //kayttoraja ei voi olla isompi kuin saldo
    if(saldo < kayttoraja)
        //pyoristetaan alempaan kymmeneen koska ei voi nostaa euroja tai sentteja
        kayttoraja = (int)saldo - ((int)saldo % 10);
    //tulostetaan tilin tiedot
    printf("\nTilin tilanne:\n");
    printf("Saldo: %.2f\n", saldo);
    printf("Tililta nostettavissa: %.2f\n", kayttoraja);
    printf("Kateisnostovara: %.2f\n", max_nosto);
    //mahdollisuus palata valikkoon tai poistua
    printf("\nValitse:\n(1) takaisin | (muu) poistu\n");
    scanf("%d", &toiminto);
    switch (toiminto){
    case 1:
        valikko(*raha);
        break;
    default:
        poistu();
        break;
    }
}

void otto(double *saldo){
    double rahat;
    int toiminto, arvo, maara, seteli_20=0, seteli_50=0, min_nosto=20, seteli=0;
    rahat = *saldo;

    //syotetaan nostettava summa
    printf("Syota nostettava summa:\n");
    //niin kauan kunnes syotetty validi summa
    while((arvo = scanf("%d", &maara)) == 0 || maara >= rahat || maara > max_nosto || maara < 20 || maara == 30 || maara % 10 != 0){
        tyhjennaPuskuri();
        if(maara > rahat)
            printf("Saldosi ei riita, syota pienempi summa\n");
        else if(maara > max_nosto)
            printf("Nostoraja on %.2lf\n", max_nosto);
        else
            printf("Invalidi summa\n");
        printf("Syota nostettava summa:\n");
    }
    printf("Nostettiin %d euroa\n\nRahat tulossa...\n\n", maara);
    //vahennetaan summa rahasta ja siirretaan muuttujaan saldo
    rahat -= maara;
    *saldo = rahat;
    for(int i = maara; i > 0; i -= seteli){
        //jos summa on jaollinen seka 50 etta 20, niin annetaan vain 50 (esim 100e)
        if(i % 50 == 0 && i % 20 == 0){
            seteli_50 += i / 50;
            seteli = i;
        }
        //jos jaollinen 20 ja alle 100e niin annetaan vain 20e seteleina
        else if(i % 20 == 0 && i < 100){
            seteli_20 += i / 20;
            seteli = i;
        }
        //jos jaollinen 50 niin annetaan vain 50e seteleina
        else if(i % 50 == 0){
            seteli_50 += i / 50;
            seteli = i;
        }
        //jos ei jaollinen 20 eika 50, tai yli 100e, vahennetaan 50e
        else{
            seteli_50 += 1;
            seteli = 50;
        }
    }
    //tulostetaan seteleiden maara vain jos niita annetaan niin nayttaa kivemmalta
    if(seteli_50 > 0)
        printf("%d kpl 50e seteleita\n", seteli_50);
    if(seteli_20 > 0)
        printf("%d kpl 20e seteleita\n", seteli_20);
    //mahdollisuus menna takaisin valikkoon tai poistua
    printf("\nValitse: (1) takaisin | (muu) poistu\n");
    scanf("%d", &toiminto);
    switch (toiminto){
    case 1:
        valikko(*saldo);
        break;
    default:
        poistu();
        break;
    }
}

void poistu(void){
    //tulostetaan vain kiitos kaynnista ja ohjelma loppuu
    printf("\nKiitos kaynnista, tervetuloa uudelleen!\n\n");
}
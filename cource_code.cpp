//son hal

#include <graphics.h>
#include<stdio.h>
#include<math.h>
#include<stdbool.h>
int yukseklik=700,genislik=700;

struct nokta
{
    float x;
    float y;
} typedef n;

struct cember
{
    nokta a;
    float R;
} typedef c;

int fact(int n)   //egri icin gerekli faktoriyel fonksiyonu
{
    if(n==0)
        return 1;
    else
        return n*fact(n-1);
}

int comb(int n,int r)        // egri icin gerekli kombinasyon fonksiyonu
{
    return(fact(n) / ( ( fact(n-r) * fact(r) ) ) );
}
int nokta_sayisi_bul(FILE *p)   //txt'de bulunan noktalarin sayisi
{

    p=fopen("noktalar.txt","r");
    if(p==0)
    {
        p=fopen("noktalar.csv","r");
    }
    int sayac=0;

    char chr;
    while (chr != EOF)                    //nokta sayisinin new line (\n) karakteri yardimiyla bulunmasi
    {

        chr=getc(p);

        if (chr == '\n')
        {
            sayac++;
        }
    }
    sayac++;
    fseek(p,0,SEEK_SET);
    fclose(p);
    return sayac;
}

void txt_oku(FILE *p,struct nokta n[],int a)     //noktalari okuyup ilgili struct dizisine aktardigimiz fonksiyonumuz
{
    p=fopen("noktalar.txt","r");
    if(p==0)
    {
        p=fopen("noktalar.csv","r");
    }
    for(int i=0; i<a; i++)
    {
        fscanf(p,"%f,%f",&n[i].x,&n[i].y);
        printf("\n%d.nokta (%.f ,%.f) ",i+1,n[i].x,n[i].y);
    }
    fseek(p,0,SEEK_SET);
    fclose(p);
}
void koordinat_cizme()                      //koordinatları 35 pozitif 35 negatif parcaya ayirdik
{
    int x1=10,y1=10,y2=0,i;
    for(x1; x1<690; x1=x1+10)
    {
        line(x1,352,x1,348);
    }
    for(y1; y1<690; y1=y1+10)
    {
        line(348,y1,352,y1);
    }
    line(350,1,343,13);
    line(350,1,357,13);
    line(350,689,357,677);
    line(350,689,343,677);
    line(1,350,13,357);
    line(1,350,13,343);
    line(689,350,676,343);
    line(689,350,676,357);


}

void cizim_asamasi(struct nokta n[],int a,c c1 )
{
    int gd=DETECT,gm;
    //  initgraph(&gd,&gm,"");
    initwindow(700,700);
    line(350,0,350,690);             //x koordinat çubugu
    line(0,350,690,350);              // y koordinat çubugy
    koordinat_cizme();
    int gecici_x=0,i=0;
    int gecici_y=0;
    //setvi
    while(i<a)                                //noktalari çizdik
    {

        gecici_x=350+10*(int)n[i].x;
        gecici_y=350-10*(int)n[i].y;
        circle(gecici_x,gecici_y,2);


        gecici_x=0;
        gecici_y=0;
        i++;
    }

    float x=350+(c1.a.x)*10 ;
    float y=350-(c1.a.y)*10;
    float r=10*c1.R;
    circle(x,y,r);        //MEC'i çizdirk
    putpixel(x,y,YELLOW);  // merkezi gösterdik
    circle(x,y,2);         //merkezi belirginleştirdik
    setcolor(3);
    line(x,y,(int)(x+r),y); //yarıcapi gosterdik
    setcolor(0);

    // BURDAN SONRAKI BOLUM NOKTALARA EN YAKIN YERLERDEN GECEN EGRIYI CIZDIRMEKTEDIR.
    float t;
    i=0,x=0,y=0;
    int m=a-1;
    for(i=0; i<=m; i++)
    {

        circle(350+n[i].x*10,350+n[i].y*-10,3);

    }
    delay(2000);

    for(t=0; t<=1; t=t+0.0002)
    {
        x=0,y=0;
        for(i=0; i<=m; i++)
        {
            x = x + (comb(m,i) * pow(1-t,m-i) * pow(t,i) * n[i].x);
            y = y + (comb(m,i) * pow(1-t,m-i) * pow(t,i) * n[i].y);
            //printf("x=%f y=%f\n",x,y);
        }
        putpixel((int)350+x*10,(int)350+y*-10,10);

    }

    getch();
    closegraph();
}


double iki_nokta_arasi_mesafe( n a,  n b)
{
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

bool noktalari_kapsar_mi(c c1, n  p)               //cember_mec_mi yardimci fonksiyon
{
    if(iki_nokta_arasi_mesafe(c1.a, p) <= c1.R)
        return true ;
    else
        return  false;
}


bool cember_mec_mi(c c, struct nokta n[],int nokta_sayisi)           //noktalarin, gecici olusan MEC'imizin icinde mi kontrolu..
{


    for (int i=0; i<nokta_sayisi; i++)
        if (!noktalari_kapsar_mi(c, n[i]))
            return false;
    return true;
}

n uc_noktanin_merkezini_bul(float bx, float by,float cx, float cy)                  //üclü inceleme icin uc noktanın orta noktasini hesaplama

{
    float B = bx * bx + by * by;
    float C = cx * cx + cy * cy;
    float D = bx * cy - by * cx;
    return { (cy * B - by * C) / (2 * D),
             (bx * C - cx * B) / (2 * D) };
}

cember uc_noktadan_cember(n  &A, n  B,  n C)
{
    n nokta1 = uc_noktanin_merkezini_bul(B.x - A.x, B.y - A.y,C.x - A.x, C.y - A.y);

    nokta1.x += A.x;
    nokta1.y += A.y;
    return { nokta1, iki_nokta_arasi_mesafe(nokta1, A) };
}



cember iki_noktadan_cember( n& A,  n& B)
{

    n C = { (A.x + B.x) / 2.0, (A.y + B.y) / 2.0 };                   //gelen iki noktanin orta noktasini hesapladik


    return { C, iki_nokta_arasi_mesafe(A, B) / 2.0 };                                           //iki nokta arasi mesafenin yarisi MEC icin r degerini verir
}



cember minimum_enclosing_circle(struct nokta n[],int a)
{

    cember mec = { { 0, 0 }, 99.0};   //minimum cemberi olusturduk ve baslangic degeri atadik.

    // Go over all pair of points
    for (int i = 0; i < a; i++)
    {
        for (int j = i + 1; j < a; j++)
        {


            cember tmp = iki_noktadan_cember(n[i], n[j]);  //dosyadan okunan noktalari ikiserli esleyerek cemberler olusturduk..


            if (tmp.R <= mec.R && cember_mec_mi(tmp,n,a))   //olusan noktalar geri kalan noktalari icine alir mi kontrolu yaptik
                mec = tmp;
        }
    }


    cember mec2= {{0,0},INFINITE};
    for (int i = 0; i < a; i++)
    {
        for (int j = i + 1; j < a; j++)
        {
            for (int k = j + 1; k < a; k++)
            {


                cember tmp = uc_noktadan_cember(n[i], n[j], n[k]);      //dosyadan okunan noktalari üclü eslestirip cemberler olusturduk

                if (tmp.R <= mec2.R && cember_mec_mi(tmp, n,a))       //olusan cember geri kalan noktalari icine alir mi kontrolu yaptik
                    mec2 = tmp;
            }
        }
    }



    printf("merkez:%.3f, %.3f  cap:%.3f  merkez2:%.3f, %.3f  cap2:%.3f \n",mec.a.x,mec.a.y,mec.R,mec2.a.x,mec2.a.y,mec2.R);
    if(mec.R<mec2.R)                             //iki farkli inceleme sonucu elimizde kalan en kucuk capli cember bizim MEC'imizdir
    {
        printf("merkez:%.3f, %.3f  cap:%.3f ",mec.a.x,mec.a.y,mec.R);
        return mec;
    }
    else
    {
        printf("merkez:%.3f,%.3f  cap:%.3f ",mec2.a.x,mec2.a.y,mec2.R);
        return mec2;
    }
}


int main()
{
    FILE *p;
    int nokta_say=nokta_sayisi_bul(p);
    struct nokta n[nokta_say];                //nokta sayisi kadar struct olusturduk
    txt_oku(p,n,nokta_say);                   //olusturulan struct'i dosyadaki veriler ile doldurduk
    printf("\nOkunan dosyadaki nokta sayisi:%d\n",nokta_say);

    cember c1= minimum_enclosing_circle(n,nokta_say);
    cizim_asamasi(n,nokta_say,c1);

    fclose(p);

    return 0;
}

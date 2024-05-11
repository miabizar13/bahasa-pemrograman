#include <iostream>
#include <string>
using namespace std;

int main(){
	
	int pilih[100],jumlah[100],sub_total[100],harga[100];
	string menu[100];
	int total_bayar;
	int i=0;
	bool selesai=false;

	
	cout<<"DAFTAR MENU JUALAN"<<endl;
    cout<<"1. Ice Tea	            Rp. 5000"<<endl;
    cout<<"2. Ice Chocolate Tea	    Rp. 8000"<<endl;
    cout<<"3. Ice Americano     Rp. 10000"<<endl;
    cout<<"4. Ice Coffee Latte	    Rp. 15000"<<endl;
    	
    	cout<<"Pilih Menu : ";
    	cin>>pilih[i];
    	
    	if (pilih[i]<=4){
    		switch (pilih[i]){
		    		case 1 : menu[i]="Ice Tea           "; harga[i]=5000; break;
		    		case 2 : menu[i]="Ice Chocolate tea    "; harga[i]=8000; break;
		    		case 3 : menu[i]="Ice Americano     "; harga[i]=10000; break;
		    		case 4 : menu[i]="Ice Coffee Latte  "; harga[i]=15000; break;
		    		default : menu[i]="";harga[i]=0;
				}
				
				cout<<"QTY "<<menu[i]<<" : ";
		    	cin>>jumlah[i];
		    	cout<<endl;
		    	
		    	sub_total[i]=jumlah[i]*harga[i];
		    	
		    	total_bayar+=sub_total[i];
	    		i++;

		} else {
			cout<<"Menu yang dipilih tidak ada"<<endl;
			cout<<endl;
		}
	
	cout<<endl;
	for (int a=0;a<i;a++){
		  cout<<a+1<<". "<<menu[a]<<" "<<sub_total[a]<<endl;
	}
	cout<<"Total Harga = Rp."<<total_bayar<<endl;
	

}
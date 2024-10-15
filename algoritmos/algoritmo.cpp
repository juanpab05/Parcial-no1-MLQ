/* 
Programa que calcula el resultado de 3 tipos de algoritmos mlq
2325093-3743, Juan Pablo Pazmiño Caicedo
*/

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <list>
#include <vector>
#include <algorithm>

using namespace std;

string nombre;
string salida;
int n=0;
int algoritmo=0;
int tiempo=0;
int duracion=0;
int quantum=0;
string linea;
int q=1;
int tq1=0;
int tq2=0;
double wt=0;
double rt=0;
double ct=0;
double tat=0;
int temp;

int tiempoCola1(int BT[], int Q[]){
    for(int i=0; i<n-2; i++){
            if(Q[i]==1){
                tq1=tq1+BT[i];
            }
        }
    return tq1;
}

int tiempoCola2(int tq1, int BT[], int Q[]){
    for(int i=0; i<n-2; i++){
            if(Q[i]==2){
                tq2=tq2+BT[i];
            }
        }
        tq2=tq2+tq1;
    return tq2;
}

//Funcion que crea un archivo de texto, y escribe en el la salida del algoritmo
void respuesta(int n, int algoritmo, string proc[], int WT[], int CT[], int RT[], int TAT[]){
    for(int i=0; i<n-2; i++){
        ct = ct + CT[i];
    }

    for(int i=0; i<n-2; i++){
        rt = rt + RT[i];
    }

    for(int i=0; i<n-2; i++){
        wt = wt + WT[i];
    }

    for(int i=0; i<n-2; i++){
        tat = tat + TAT[i];
    }

    ofstream outfile(salida);
    switch(algoritmo){
        case 1: {
            outfile<<"# Archivo: "<<"out_"<<nombre<<"; MLQ: RR(1), RR(3), SJF"<<endl;
        }
        break;
        case 2:{
            outfile<<"# Archivo: "<<"out_"<<nombre<<"; MLQ: RR(3), RR(5), FCFS"<<endl;
        }
        break;
        case 3:{
            outfile<<"# Archivo: "<<"out_"<<nombre<<"; MLQ: RR(2), RR(3), STCF"<<endl;
        }
        break;
    }
    
    outfile<<"# etiqueta; BT; AT; Q; Pr; WT; CT; RT; TAT"<<endl;
    for(int i=2; i<n; i++){
        outfile<<proc[i]<<"; "<<WT[i-2]<<"; "<<CT[i-2]<<"; "<<RT[i-2]<<"; "<<TAT[i-2]<<endl;
    }
    outfile<<"WT: "<<wt/(n-2)<<"; CT: "<<ct/(n-2)<<"; RT: "<<rt/(n-2)<<"; TAT: "<<tat/(n-2)<<endl;
}

int main(){ 
    //Se pregunta el nombre de un archivo de texto que debe estar ubicado en la carpeta raiz del programa
    cout<<"Ingrese el nombre del archivo: ";
    cin>>nombre;
    salida="out_"+nombre+".txt";
    cout<<"Numero de procesos: ";
    cin>>n;
    n=n+2;
    string proc[n];
    int AT[n-2];
    int BT[n-2];
    int BT2[n-2];
    int Q[n-2];
    int PR[n-2];
    int WT[n-2];
    int RT[n-2];
    int CT[n-2];
    int TAT[n-2];
    vector<int>COLA;
    vector<int>COLAQ;
 
    //Se leen las lineas mediante la funcion ifstream y se almacenan en el string archivo
    ifstream archivo(nombre+".txt");
    for(int i=0; i<n; i++){
        getline(archivo, linea);
        proc[i]=linea;
    }

    //Leer los valores del archivo
    for(int i=2; i<n; i++){
        size_t startBT = proc[i].find(";");
        size_t endBT = proc[i].find(";", startBT+1);
        BT[i-2]=stoi(proc[i].substr(startBT+1, endBT-startBT-1));
        BT2[i-2]=stoi(proc[i].substr(startBT+1, endBT-startBT-1));
        size_t startAT = proc[i].find(";", endBT);
        size_t endAT = proc[i].find(";", startAT+1);
        AT[i-2]=stoi(proc[i].substr(startAT+1, endAT-startAT-1));
        size_t startQ = proc[i].find(";", endAT);
        size_t endQ = proc[i].find(";", startQ+1);
        Q[i-2]=stoi(proc[i].substr(startQ+1, endQ-startQ-1));
        size_t startPR = proc[i].find(";", endQ);
        size_t endPR = startPR+3;
        PR[i-2]=stoi(proc[i].substr(startPR+1, endPR-startPR-1));
    }

    //Inicializar:
    for(int i=0; i<n-2; i++){
        WT[i]=0;
    }

    cout<<"Escoja un algoritmo: "<<endl;
    cout<<"1. RR(1), RR(3), SJF"<<endl;
    cout<<"2. RR(3), RR(5), FCFS"<<endl;
    cout<<"3. RR(2), RR(3), STCF"<<endl;
    cin>>algoritmo;
    
switch(algoritmo){
    //RR(1), RR(3), SJF
    case 1:{
        tq1= tiempoCola1(BT, Q);
        tq2= tiempoCola2(tq1, BT, Q);

    //Tiempo total de corrida del programa
        for(int i=0; i<n-2; i++){
            duracion=duracion+BT[i];
        }

        while(tiempo<duracion){
            //Se reinicia la cola para evitar la acumulacion de elementos
            COLA.erase(COLA.begin(), COLA.end());
            if(q=1){
                quantum=1;
            }
            //Pasar a la cola siguiente
            if(tiempo>=tq1){
                q=2;
                quantum=3;
            }
            if(tiempo>=tq2){
                q=3;
            }

            //Agregar elementos a las 2 colas:
            for(int a=0; a<n-2; a++){
                if(AT[a]<=tiempo && BT2[a]>0){
                    COLA.push_back(a);
                    if(Q[a]==q){
                        COLAQ.push_back(a);
                    }
                }   
            }
            
            //En caso de que en t=0 no haya ningun proceso de la primera cola
            if(COLAQ.size()==0){
                for(int a=0; a<n-2; a++){
                    if(AT[a]<=tiempo && BT2[a]>0 && Q[a]==2){
                        COLAQ.push_back(a);
                    }
                }
            }
            if(COLAQ.size()==0){
                for(int a=0; a<n-2; a++){
                    if(AT[a]<=tiempo && BT2[a]>0 && Q[a]==3){
                        COLAQ.push_back(a);
                    }
                }
            }

        //Organizar la cola por orden de BT restante menor (Parte del SJF)
            if(COLAQ.size()>1 && Q[COLAQ.at(0)]==3){
                int size=COLAQ.size();  
                //Cola secundaria temporal 
                int COLA2[size];
                for(int y=0; y<size; y++){
                    COLA2[y]=COLAQ.at(y);
                }
                //Se organiza la cola secundaria por BT mas bajo
                for(int b=0; b<size; b++){
                    for(int y=0; y<size-(1+b); y++){
                        if(BT2[COLA2[y]]>BT2[COLA2[y+1]]){
                            temp=COLA2[y];
                            COLA2[y]=COLA2[y+1];
                            COLA2[y+1]=temp;
                        }
                    }
                }
                //Se borran los valores de la cola primaria, y se agregan los de la secundaria ordenados.
                COLAQ.erase(COLAQ.begin(), COLAQ.end()); 
                for(int d=0; d<size; d++){
                    COLAQ.push_back(COLA2[d]);
                } 
            }

        int id=COLAQ.at(0);
        //Se asigna al rt tiempo cuando el proceso llega por primera vez al ciclo, 
        //osea que su BT2 no ha sido modificado
        if(BT[id]==BT2[id]){
            RT[id]=tiempo;
        }

        //Procesamiento tipo round robin
        if(Q[id]==1 || Q[id]==2){
            //Esto es en caso de que haya un proceso en cola que no pertenece a la actual,
            //Asi este puede tener el quantum que le corresponde a su cola.
            if(Q[id]==1){
                quantum=1;
            } else{
                quantum=3;
            }
            while(quantum!=0){
                for(int a=0; a<COLA.size(); a++){
                    if(COLA[a]!=id){
                        WT[COLA.at(a)]=WT[COLA.at(a)]+1;
                    }
                }
                quantum=quantum-1;
                tiempo=tiempo+1;
                BT2[id]=BT2[id]-1;
                if(BT2[id]==0){
                    CT[id]=tiempo;
                    TAT[id]=tiempo-AT[id];
                    quantum=0;
                }
                //Si se procesa un proceso que no pertenece a la cola actual, entonces es necesario  
                //modificar los tiempos cuando se cambia de lista.
                if(Q[id]!=q){
                    if(q==1){
                        tq1=tq1+1;
                        tq2=tq2+1;
                    }
                    if(q==2){
                        tq2=tq2+1;
                    }
                }
                if((BT2[id]==0 && COLAQ.size()>1) || Q[id]!=q){
                    COLAQ.erase(COLAQ.begin(), COLAQ.end());
                }
            }
            if(COLAQ.size()>0){
                COLAQ.erase(COLAQ.begin());
            }  
        }
        
        //Procesamiento SJF

        //Calcular wt, ct y tat para SJF   
        if(Q[id]==3){ 
            COLAQ.erase(COLAQ.begin());
            WT[id]=tiempo-AT[id];
            tiempo=tiempo+BT2[id];
            CT[id]=tiempo;
            TAT[id]=tiempo-AT[id];
            //En caso de que hayan elementos en cola que no pertenecen a la cola actual.
            if(Q[id]!=q){
                COLAQ.erase(COLAQ.begin(), COLAQ.end());
                if(q==1){
                    tq1=tq1+BT2[id];
                    tq2=tq2+BT2[id];
                }
                if(q==2){
                    tq2=tq2+BT2[id];
                }
            }
            BT2[id]=0;
        }
    }
    //Llamar a la funcion respuesta para finalizar el programa
    respuesta(n, algoritmo, proc, WT, CT, RT, TAT);
    }
    break;


    //RR(3), RR(5), FCFS:
    case 2:{
        tq1= tiempoCola1(BT, Q);
        tq2= tiempoCola2(tq1, BT, Q);

    //Tiempo total de corrida del programa
        for(int i=0; i<n-2; i++){
            duracion=duracion+BT[i];
        }

        while(tiempo<duracion){
            COLA.erase(COLA.begin(), COLA.end());
            if(q=1){
                quantum=3;
            }
            if(tiempo>=tq1){
                q=2;
                quantum=5;
            }
            if(tiempo>=tq2){
                q=3;
            }

            //Agregar elementos a la cola actual
            for(int a=0; a<n-2; a++){
                if(AT[a]<=tiempo && BT2[a]>0){
                    COLA.push_back(a);
                    if(Q[a]==q){
                        COLAQ.push_back(a);
                    }
                } 
            }

            //En caso de que en t=0 no haya ningun proceso de la primera cola
            if(COLAQ.size()==0){
                for(int a=0; a<n-2; a++){
                    if(AT[a]<=tiempo && BT2[a]>0 && Q[a]==2){
                        COLAQ.push_back(a);
                    }
                }
            }
            if(COLAQ.size()==0){
                for(int a=0; a<n-2; a++){
                    if(AT[a]<=tiempo && BT2[a]>0 && Q[a]==3){
                        COLAQ.push_back(a);
                    }
                }
            }
        
        int id=COLAQ.at(0);
        if(BT[id]==BT2[id]){
            RT[id]=tiempo;
        }

        if(Q[id]==1 || Q[id]==2){
            //Esto es en caso de que haya un proceso en cola que no pertenece a la actual,
            //Asi este puede tener el quantum que le corresponde a su cola.
            if(Q[id]==1){
                quantum=3;
            } else{
                quantum=5;
            }
            while(quantum!=0){
                for(int a=0; a<COLA.size(); a++){
                    if(COLA[a]!=id){
                        WT[COLA.at(a)]=WT[COLA.at(a)]+1;
                    }
                }
                quantum=quantum-1;
                tiempo=tiempo+1;
                BT2[id]=BT2[id]-1;
                if(BT2[id]==0){
                    CT[id]=tiempo;
                    TAT[id]=tiempo-AT[id];
                    quantum=0;
                }
                //Si se procesa un proceso que no pertenece a la cola actual, entonces es necesario  
                //modificar los tiempos cuando se cambia de lista.
                if(Q[id]!=q){
                    if(q==1){
                        tq1=tq1+1;
                        tq2=tq2+1;
                    }
                    if(q==2){
                        tq2=tq2+1;
                    }
                }
                if(BT2[id]==0 && COLAQ.size()>1 || Q[id]!=q){
                    COLAQ.erase(COLAQ.begin(), COLAQ.end());
                }
            }
            if(COLAQ.size()>0){
                COLAQ.erase(COLAQ.begin());
            }  
        }

        //Calcular wt, ct y tat para FCFS
        if(Q[id]==3){
            COLAQ.erase(COLAQ.begin());
            WT[id]=tiempo-AT[id];
            tiempo=tiempo+BT2[id];
            CT[id]=tiempo;
            TAT[id]=tiempo-AT[id];
            if(Q[id]!=q){
                COLAQ.erase(COLAQ.begin(), COLAQ.end());
                if(q==1){
                    tq1=tq1+BT2[id];
                    tq2=tq2+BT2[id];
                }
                if(q==2){
                    tq2=tq2+BT2[id];
                }
            }
            BT2[id]=0;
        }
    }

    respuesta(n, algoritmo, proc, WT, CT, RT, TAT);
    }
    break;

    //RR(2), RR(3), STCF:
    case 3:{
        tq1= tiempoCola1(BT, Q);
        tq2= tiempoCola2(tq1, BT, Q);

    //Tiempo total de corrida del programa
        for(int i=0; i<n-2; i++){
            duracion=duracion+BT[i];
        }

        while(tiempo<duracion){
            COLA.erase(COLA.begin(), COLA.end());
            if(q=1){
                quantum=2;
            }
            if(tiempo>=tq1){
                q=2;
                quantum=3;
            }
            if(tiempo>=tq2){
                q=3;
                //Si la cola es 3, para que no se acumulen los elementos puedo simplemente reinciarla, 
                //ya que a diferencia del round robin, en estos casos no es necesario que se este intercalando los procesos
                COLAQ.erase(COLAQ.begin(), COLAQ.end());
            }

            //Agregar elementos a las colas
            for(int a=0; a<n-2; a++){
                if(AT[a]<=tiempo && BT2[a]>0){
                    COLA.push_back(a);
                    if(Q[a]==q){
                        COLAQ.push_back(a);
                    }
                } 
            }

            //En caso de que en t=0 no haya ningun proceso de la primera cola
            if(COLAQ.size()==0){
                for(int a=0; a<n-2; a++){
                    if(AT[a]<=tiempo && BT2[a]>0 && Q[a]==2){
                        COLAQ.push_back(a);
                    }
                }
            }
            if(COLAQ.size()==0){
                for(int a=0; a<n-2; a++){
                    if(AT[a]<=tiempo && BT2[a]>0 && Q[a]==3){
                        COLAQ.push_back(a);
                    }
                }
            }

        //Ordenar la cola por BT2 mas corto (parte de SJF y STCF)
        if(COLAQ.size()>1 && Q[COLAQ.at(0)]==3){
            int size=COLAQ.size();  
            //Cola secundaria temporal 
            int COLA2[size];
            for(int y=0; y<size; y++){
                COLA2[y]=COLAQ.at(y);
            }
            //Se organiza la cola secundaria por BT mas bajo
            for(int b=0; b<size; b++){
                for(int y=0; y<size-(1+b); y++){
                    if(BT2[COLA2[y]]>BT2[COLA2[y+1]]){
                        temp=COLA2[y];
                        COLA2[y]=COLA2[y+1];
                        COLA2[y+1]=temp;
                    }

                }
            }
            //Se borran los valores de la cola primaria, y se agregan los de la secundaria ordenados.
            COLAQ.erase(COLAQ.begin(), COLAQ.end()); 
            for(int d=0; d<size; d++){
                COLAQ.push_back(COLA2[d]);
            } 
        }

        int id=COLAQ.at(0);
        //Calcular RT
        if(BT[id]==BT2[id]){
            RT[id]=tiempo;
        }

        //Procesamiento round robin
        if(Q[id]==1 || Q[id]==2){
            //Esto es en caso de que haya un proceso en cola que no pertenece a la actual,
            //Asi este puede tener el quantum que le corresponde a su cola.
            if(Q[id]==1){
                quantum=2;
            } else{
                quantum=3;
            }
            while(quantum!=0){
                for(int a=0; a<COLA.size(); a++){
                    if(COLA[a]!=id){
                        WT[COLA.at(a)]=WT[COLA.at(a)]+1;
                    }
                }
                quantum=quantum-1;
                tiempo=tiempo+1;
                BT2[id]=BT2[id]-1;
                if(BT2[id]==0){
                    CT[id]=tiempo;
                    TAT[id]=tiempo-AT[id];
                    quantum=0;
                }
                //Si se procesa un proceso que no pertenece a la cola actual, entonces es necesario  
                //modificar los tiempos cuando se cambia de lista.
                if(Q[id]!=q){
                    tq1=tq1+1;
                    tq2=tq2+1;
                }
                if(BT2[id]==0 && COLAQ.size()>1 || Q[id]!=q){
                    COLAQ.erase(COLAQ.begin(), COLAQ.end());
                }
            }
            if(COLAQ.size()>0){
                COLAQ.erase(COLAQ.begin());
            }
        }
        
        //Procesamiento STCF

        //Calcular WT, CT y TAT para STCF
        if(Q[id]==3){
            tiempo=tiempo+1; 
            BT2[id]=BT2[id]-1;
            if(BT2[id]==0){
                COLAQ.erase(COLAQ.begin());
                CT[id]=tiempo;
                TAT[id]=CT[id]-AT[id];  
            }
            //Agregar wait times:
            for(int a=0; a<COLA.size(); a++){
                if(COLA[a]!=id){
                    WT[COLA.at(a)]=WT[COLA.at(a)]+1;
                }
            }
            //Aumentar tiempo de las colas, y reiniciar la cola 
            //en caso de que el proceso no pertenezca a la cola actual
            if(Q[id]!=q){
                COLAQ.erase(COLAQ.begin(), COLAQ.end());
                if(q==1){
                    tq1=tq1+1;
                    tq2=tq2+1;
                }
                if(q==2){
                    tq2=tq2+1;
                }
            }
        }
    }

    respuesta(n, algoritmo, proc, WT, CT, RT, TAT);
    }
    break;

    }

}
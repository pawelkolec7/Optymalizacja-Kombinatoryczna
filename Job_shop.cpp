#include <iostream>
#include <vector>
#include <queue>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <fstream>
#include <cstring>
using namespace std;

struct operation{
    int machine;
    int processing_time;
};

void pomieszaj(int tablica[], int n)
{
    int temp = 0;
    int randomIndex = 0;

    for(int i=0; i<n; i++)
    {
        randomIndex = rand()%n;
        temp = tablica[i];
        tablica[i] = tablica[randomIndex];
        tablica[randomIndex] = temp;
    }
}

void Beasley(int argc, char *argv[], int x)
{
    ifstream plik(argv[1]);

    int numJobs, numMachines;

    plik >> numJobs >> numMachines;
    int num_operations = numJobs*numMachines;
    vector<queue<operation> > kolejka_zadania(numJobs);
    int max_machine = 0;

    operation operacja_x;
    for(int i=0; i<numJobs; i++) {
        for (int j = 0; j < numMachines; j++) {
            plik >> operacja_x.machine >> operacja_x.processing_time;
            if(operacja_x.machine>max_machine)
                max_machine = operacja_x.machine;
            kolejka_zadania[i].push(operacja_x);
        }}

    int final_solution[numJobs][numMachines];
    int final_makespan = -1;

    int dostepne_liczby[numJobs]; //dodatkowa tablica do losowania
    for(int i=0;i<numJobs;i++)
        dostepne_liczby[i] = i;

    int iteracje = x;
    auto start = std::chrono::high_resolution_clock::now();
    //ALGORYTM
    while (std::chrono::high_resolution_clock::now() - start < 180s && iteracje>0)
    {
        //zmienne pomocnicze do rozpatrywania czy zadanie/maszyna jest zajete
        int machines[max_machine+1];
        for (int i = 0; i < max_machine+1; i++) {
            machines[i] = 0;
        }
        int jobs[numJobs];
        for (int i = 0; i < numJobs; i++) {
            jobs[i] = 0;
        }
        int left_time[numJobs];
        for (int i = 0; i < numJobs; i++) {
            left_time[i] = 0;
        }
        int zajmowane_maszyny[numJobs];
        int ilosc_operacji[numJobs];
        for (int i = 0; i < numJobs; i++) {
            ilosc_operacji[i] = 0;
        }
        int solution[numJobs][numMachines]; //macierz dla tymczasowego rozwiazania

        int global_time = 0; //zmienna do przechowania jednostek czasu
        num_operations = numJobs*numMachines; //laczna ilosc operacji
        vector<queue<operation> > kopia = kolejka_zadania; //kopia kolejki, zeby nie usuwac elementow z oryginalnej


        while(num_operations>0)
        {
            pomieszaj(dostepne_liczby,numJobs); //losowanie prac
            bool czy_bylo_zero = false;
            for(int i=0;i<numJobs;i++)
            {
                int r = dostepne_liczby[i]; //losowanie w przedziale od 0 do liczby prac
                if(!kopia[r].empty()) //warunek sprawdzajacy czy kolejka wylosowanego zadania nie jest pusta
                {
                    if(jobs[r] == 0) //warunek sprawdzajacy czy wylosowane zadania jest wolne
                    {
                        operacja_x = kopia[r].front(); //pobranie operacji z kolejki
                        if(machines[operacja_x.machine] == 0) //warunek sprawdzajacy czy maszyna jest wolna
                        {
                            jobs[r] = 1;
                            machines[operacja_x.machine] = 1;
                            left_time[r] = operacja_x.processing_time;
                            zajmowane_maszyny[r] = operacja_x.machine;
                            kopia[r].pop(); //usuniecie operacji z kolejki
                            solution[r][ilosc_operacji[r]] = global_time;
                            ilosc_operacji[r]++;
                            if(left_time[r] == 0) //to sprawda czy nie ma operacji z czasem 0
                            {
                                czy_bylo_zero = true;
                                jobs[r] = 0;
                                machines[zajmowane_maszyny[r]] = 0;
                                num_operations--;
                            }
                        }
                    }
                }
            }
            int najmniejszy_left_time;
            bool first = true;
            bool czy_cos_sie_zakonczylo = false;
            if(czy_bylo_zero == 0) //jesli byla jakakolwiek operacja z czasem 0, to pomija sprawdzanie innych
            {
                for(int i=0;i<numJobs;i++)
                {
                    if(left_time[i]>1) //operacja nadal trwa
                    {
                        left_time[i]--;
                        if(left_time[i]<najmniejszy_left_time || first == 1)
                        {
                            najmniejszy_left_time = left_time[i];
                            first = false;
                        }

                    }
                    else if(jobs[i] == 1 && left_time[i] == 1) //operacja powinna byc zakonczona
                    {
                        jobs[i] = 0;
                        machines[zajmowane_maszyny[i]] = 0;

                        left_time[i] = 0;
                        num_operations--;
                        czy_cos_sie_zakonczylo = true;
                    }
                }
                if(czy_cos_sie_zakonczylo == 0) //jesli kazda operacja nadal trwa
                {
                    for(int i=0; i<numJobs; i++)
                        if(left_time[i]>0)
                            left_time[i] = left_time[i] - najmniejszy_left_time + 1;
                    global_time+=najmniejszy_left_time;
                }
                else
                    global_time++;
            }
        }

        //dodanie pierwszego rozwiazania, ktore obliczyl algorytm do koncowego rozwiazania
        if(final_makespan == -1)
        {
            for(int i=0;i<numJobs;i++)
            {
                for(int j=0;j<numMachines;j++)
                {
                    final_solution[i][j] =solution[i][j];
                }
            }
            final_makespan = global_time;
        }
        else if(global_time<final_makespan) //tutaj jesli rozwiazanie jest krotsze od aktualnie najlepszego to podmienia
        {
            for(int i=0;i<numJobs;i++)
            {
                for(int j=0;j<numMachines;j++)
                {
                    final_solution[i][j] =solution[i][j];
                }
            }
            final_makespan = global_time;
        }
        iteracje--;
    }


    //WYPISANIE WYNIKU
    ofstream wynik(argv[2]);
    wynik<<final_makespan<<"\n";
    for(int i=0;i<numJobs;i++)
    {
        for(int j=0;j<numMachines;j++)
        {
            wynik<<final_solution[i][j]<<" ";
        }
        wynik<<"\n";
    }
    plik.close();
    wynik.close();
}

void Taillard(int argc, char *argv[], int x)
{
    ifstream plik(argv[1]);

    int numJobs, numMachines;
    plik >> numJobs >> numMachines;
    int num_operations = numJobs*numMachines;
    vector<queue<operation> > kolejka_zadania(numJobs);
    int max_machine = 0;

    string uselless; //do utylizacji
    plik>>uselless>>uselless>>uselless>>uselless; //eliminowanie niepotrzebnych zmiennych
    plik>>uselless; //"Times

    //Tablice do tymczasowego przechowania czasow i maszyn z pliku
    int tmp_times[numJobs][numMachines];
    int tmp_machines[numJobs][numMachines];

    for(int i=0; i<numJobs; i++){
        for (int j = 0; j < numMachines; j++) {
            plik >> tmp_times[i][j];
        }
    }

    plik>>uselless; //"Machines"

    for(int i=0; i<numJobs; i++){
        for (int j = 0; j < numMachines; j++) {
            plik >> tmp_machines[i][j];
        }
    }

    ofstream beasley("beasley.txt"); //plik ktory przechowuje przekonwertowane intancje Tailarda na format Beasleya (dla sprawdzarki)
    beasley<<numJobs<<" "<<numMachines<<"\n";
    operation operacja_x;
    for(int i=0; i<numJobs; i++) {
        for (int j = 0; j < numMachines; j++) {
            operacja_x.machine = tmp_machines[i][j];
            operacja_x.processing_time = tmp_times[i][j];
            if(operacja_x.machine>max_machine)
                max_machine = operacja_x.machine;
            beasley<<operacja_x.machine<<" "<<operacja_x.processing_time<<" ";
            kolejka_zadania[i].push(operacja_x);
        }
        beasley<<"\n";
    }

    int final_solution[numJobs][numMachines];
    int final_makespan = -1;

    int dostepne_liczby[numJobs]; //dodatkowa tablica do losowania
    for(int i=0;i<numJobs;i++)
        dostepne_liczby[i] = i;

    int iteracje = x;
    auto start = std::chrono::high_resolution_clock::now();
    //ALGORYTM
    while (std::chrono::high_resolution_clock::now() - start < 180s && iteracje>0)
    {
        //zmienne pomocnicze do rozpatrywania czy zadanie/maszyna jest zajete
        int machines[max_machine+1];
        for (int i = 0; i < max_machine+1; i++) {
            machines[i] = 0;
        }
        int jobs[numJobs];
        for (int i = 0; i < numJobs; i++) {
            jobs[i] = 0;
        }
        int left_time[numJobs];
        for (int i = 0; i < numJobs; i++) {
            left_time[i] = 0;
        }
        int zajmowane_maszyny[numJobs];
        int ilosc_operacji[numJobs];
        for (int i = 0; i < numJobs; i++) {
            ilosc_operacji[i] = 0;
        }
        int solution[numJobs][numMachines];

        int global_time = 0;
        num_operations = numJobs*numMachines;
        vector<queue<operation> > kopia = kolejka_zadania;

        while(num_operations>0)
        {
            pomieszaj(dostepne_liczby,numJobs);
            bool czy_bylo_zero = false;
            for(int i=0;i<numJobs;i++)
            {
                int r = dostepne_liczby[i];
                if(!kopia[r].empty()) //warunek sprawdzajacy czy kolejka wylosowanego zadania nie jest pusta
                {
                    if(jobs[r] == 0) //warunek sprawdzajacy czy wylosowane zadania jest wolne
                    {
                        operacja_x = kopia[r].front(); //pobranie operacji z kolejki
                        if(machines[operacja_x.machine-1] == 0) //warunek sprawdzajacy czy maszyna jest wolna
                        {
                            jobs[r] = 1; //jobs[1] = 1
                            machines[operacja_x.machine-1] = 1;
                            left_time[r] = operacja_x.processing_time;
                            zajmowane_maszyny[r] = operacja_x.machine-1;
                            kopia[r].pop(); //usuniecie z kolejki
                            solution[r][ilosc_operacji[r]] = global_time;
                            ilosc_operacji[r]++;
                            if(left_time[r] == 0) //to sprawda czy nie ma operacji z czasem 0
                            {
                                czy_bylo_zero = true;
                                jobs[r] = 0;
                                machines[zajmowane_maszyny[r]] = 0;
                                num_operations--;
                            }
                        }
                    }
                }
            }
            int najmniejszy_left_time;
            bool first = true;
            bool czy_cos_sie_zakonczylo = false;
            if(czy_bylo_zero == 0) //jesli byla jakakolwiek operacja z czasem 0, to pomija sprawdzanie innych
            {
                for(int i=0;i<numJobs;i++)
                {
                    if(left_time[i]>1) //operacja nadal trwa
                    {
                        left_time[i]--;
                        if(left_time[i]<najmniejszy_left_time || first == 1)
                        {
                            najmniejszy_left_time = left_time[i];
                            first = false;
                        }

                    }
                    else if(jobs[i] == 1 && left_time[i] == 1) //operacja powinna byc zakonczona
                    {
                        jobs[i] = 0;
                        machines[zajmowane_maszyny[i]] = 0;

                        left_time[i] = 0;
                        num_operations--;
                        czy_cos_sie_zakonczylo = true;
                    }
                }
                if(czy_cos_sie_zakonczylo == 0) //jesli kazda operacja nadal trwa
                {
                    for(int i=0; i<numJobs; i++)
                        if(left_time[i]>0)
                            left_time[i] = left_time[i] - najmniejszy_left_time + 1;
                    global_time+=najmniejszy_left_time;
                }
                else
                    global_time++;
            }
        }
        if(final_makespan == -1)
        {
            for(int i=0;i<numJobs;i++)
            {
                for(int j=0;j<numMachines;j++)
                {
                    final_solution[i][j] =solution[i][j];
                }
            }
            final_makespan = global_time;
        }
        else if(global_time<final_makespan)
        {
            for(int i=0;i<numJobs;i++)
            {
                for(int j=0;j<numMachines;j++)
                {
                    final_solution[i][j] =solution[i][j];
                }
            }
            final_makespan = global_time;
        }
        iteracje--;
    }
    //WYPISANIE WYNIKU
    ofstream wynik (argv[2]);
    wynik<<final_makespan<<endl;
    for(int i=0;i<numJobs;i++)
    {
        for(int j=0;j<numMachines;j++)
        {
            wynik<<final_solution[i][j]<<" ";
        }
        wynik<<endl;
    }
    plik.close();
    beasley.close();
    wynik.close();
}
int main(int argc, char *argv[]) {
    srand(time(NULL));

    int x = 1000; //reczne ustawienie iteracji algorytmu

    /*argv[1] = "input.txt";
    argv[2] = "output.txt";
    argv[3] = "T";*/

    if (strcmp(argv[3], "B") == 0)
        Beasley(argc,argv,x);
    else if (strcmp(argv[3], "T") == 0)
        Taillard(argc,argv,x);
    return 0;
}
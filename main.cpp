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

void Beasley(int argc, char *argv[])
{
    ifstream plik(argv[1]);

    int numJobs, numMachines;


    plik >> numJobs >> numMachines;
    int num_operations = numJobs*numMachines;
    vector<queue<operation> > kolejka_zadania(numJobs);

    operation operacja_x;
    for(int i=0; i<numJobs; i++) {
        for (int j = 0; j < numMachines; j++) {
            plik >> operacja_x.machine >> operacja_x.processing_time;
            kolejka_zadania[i].push(operacja_x);
        }}


    /*// Wypisywanie wczytanych danych
    for (int i = 0; i < numJobs; i++) {
        while (!kolejka_zadania[i].empty()) {
            operacja_x = kolejka_zadania[i].front();
            kolejka_zadania[i].pop();
            cout << operacja_x.machine << " " << operacja_x.processing_time<<" ";
        }
        cout<<endl;
    }*/
    int final_solution[numJobs][numMachines];
    int final_makespan = -1;

    int iteracje = 100;
    auto start = std::chrono::high_resolution_clock::now();
    while (std::chrono::high_resolution_clock::now() - start < 180s and iteracje>0)
    {

        int global_time = 0;
        num_operations = numJobs*numMachines;
        vector<queue<operation> > kopia = kolejka_zadania;

        //zmienne pomocnicze do rozpatrywania czy zadanie/maszyna jest zajete
        int machines[numMachines];
        for (int i = 0; i < numMachines; i++) {
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

        while(num_operations>0)
        {
            for(int i=0;i<numJobs*numMachines;i++)
            {
                int r = rand()%numJobs;
                if(!kopia[r].empty())
                {
                    if(jobs[r] == 0)
                    {
                        operacja_x = kopia[r].front();
                        if(machines[operacja_x.machine] == 0)
                        {
                            //Zalozmy wylosowalo 1 2
                            jobs[r] = 1; //jobs[1] = 1
                            machines[operacja_x.machine] = 1; //machines[1] = 1
                            left_time[r] = operacja_x.processing_time; //left_time[1] = 2
                            zajmowane_maszyny[r] = operacja_x.machine;
                            kopia[r].pop();
                            solution[r][ilosc_operacji[r]] = global_time;
                            ilosc_operacji[r]++;

                        }
                    }
                }
            }

            for(int i=0;i<numJobs;i++)
            {
                if(left_time[i] == 0 && jobs[i] == 1) //to sprawda czy nie ma operacji z czasem 0
                {
                    jobs[i] = 0;
                    machines[zajmowane_maszyny[i]] = 0;

                    left_time[i] = 0;
                    num_operations--;
                }
                else if(left_time[i]>1)
                    left_time[i]--;
                else if(jobs[i] == 1 && left_time[i] == 1)
                {
                    jobs[i] = 0;
                    machines[zajmowane_maszyny[i]] = 0;

                    left_time[i] = 0;
                    num_operations--;
                }
            }
            global_time++;
        }
        /*cout<<global_time<<endl;
        for(int i=0;i<numJobs;i++)
        {
            for(int j=0;j<numMachines;j++)
            {
                cout<<solution[i][j]<<" ";
            }
            cout<<endl;
        }*/
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
}

void Taillard()
{
    int numJobs, numMachines;
    cin >> numJobs >> numMachines;
    int num_operations = numJobs*numMachines;
    vector<queue<operation> > kolejka_zadania(numJobs);

    string uselless; //do utylizacji
    cin>>uselless>>uselless>>uselless>>uselless; //eliminowanie niepotrzebnych zmiennych
    cin>>uselless;

    int tmp_times[numJobs][numMachines];
    int tmp_machines[numJobs][numMachines];

    for(int i=0; i<numJobs; i++){
        for (int j = 0; j < numMachines; j++) {
            cin >> tmp_times[i][j];
        }
    }

    cin>>uselless; //"Machines"

    for(int i=0; i<numJobs; i++){
        for (int j = 0; j < numMachines; j++) {
            cin >> tmp_machines[i][j];
        }
    }

    operation operacja_x;
    for(int i=0; i<numJobs; i++) {
        for (int j = 0; j < numMachines; j++) {
            operacja_x.machine = tmp_machines[i][j];
            operacja_x.processing_time = tmp_times[i][j];
            kolejka_zadania[i].push(operacja_x);
        }}

    /*// Wypisywanie wczytanych danych
    for (int i = 0; i < numJobs; i++) {
        while (!kolejka_zadania[i].empty()) {
            operacja_x = kolejka_zadania[i].front();
            kolejka_zadania[i].pop();
            cout << operacja_x.machine << " " << operacja_x.processing_time<<" ";
        }
        cout<<endl;
    }*/
    int final_solution[numJobs][numMachines];
    int final_makespan = -1;

    int iteracje = 100;
    auto start = std::chrono::high_resolution_clock::now();
    while (std::chrono::high_resolution_clock::now() - start < 180s and iteracje>0)
    {

        int global_time = 0;
        num_operations = numJobs*numMachines;
        vector<queue<operation> > kopia = kolejka_zadania;

        //zmienne pomocnicze do rozpatrywania czy zadanie/maszyna jest zajete
        int machines[numMachines];
        for (int i = 0; i < numMachines; i++) {
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

        while(num_operations>0)
        {
            for(int i=0;i<numJobs*numMachines;i++)
            {
                int r = rand()%numJobs;
                if(!kopia[r].empty())
                {
                    if(jobs[r] == 0)
                    {
                        operacja_x = kopia[r].front();
                        if(machines[operacja_x.machine] == 0)
                        {
                            //Zalozmy wylosowalo 1 2
                            jobs[r] = 1; //jobs[1] = 1
                            machines[operacja_x.machine] = 1; //machines[1] = 1
                            left_time[r] = operacja_x.processing_time; //left_time[1] = 2
                            zajmowane_maszyny[r] = operacja_x.machine;
                            kopia[r].pop();
                            solution[r][ilosc_operacji[r]] = global_time;
                            ilosc_operacji[r]++;

                        }
                    }
                }
            }

            for(int i=0;i<numJobs;i++)
            {
                if(left_time[i] == 0 && jobs[i] == 1) //to sprawda czy nie ma operacji z czasem 0
                {
                    jobs[i] = 0;
                    machines[zajmowane_maszyny[i]] = 0;

                    left_time[i] = 0;
                    num_operations--;
                }
                else if(left_time[i]>1)
                    left_time[i]--;
                else if(jobs[i] == 1 && left_time[i] == 1)
                {
                    jobs[i] = 0;
                    machines[zajmowane_maszyny[i]] = 0;

                    left_time[i] = 0;
                    num_operations--;
                }
            }
            global_time++;
        }
        /*cout<<global_time<<endl;
        for(int i=0;i<numJobs;i++)
        {
            for(int j=0;j<numMachines;j++)
            {
                cout<<solution[i][j]<<" ";
            }
            cout<<endl;
        }*/
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
    cout<<final_makespan<<endl;
    for(int i=0;i<numJobs;i++)
    {
        for(int j=0;j<numMachines;j++)
        {
            cout<<final_solution[i][j]<<" ";
        }
        cout<<endl;
    }


}
int main(int argc, char *argv[]) {
    srand(time(NULL));
    /*argv[1] = "input.txt";
    argv[2] = "output.txt";
    argv[3] = "B";*/
    if (strcmp(argv[3], "B") == 0)
        Beasley(argc,argv);

    //else if (argv[3] == 'T')
    //    Taillard();
    return 0;
}
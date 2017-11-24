#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <queue>
#include<algorithm>
//#define size 99999;
using namespace std;
#define INFINITY 65535
//int arr[9999];
//global declaration
long size, src,dest = -10, dn;

long **graph;
long *interfc;
long *dist;
long **path;
long **cost;
long *distanc;
long *pred;
int *check;
string fileName;
struct nodeDistance
{
    int node;
    unsigned int distance;
};


 //function to calculate number of columns
int ReadNumbers( const string & s, vector <double> & v ) {
    istringstream is( s );
    double n;
    while( is >> n ) {
        v.push_back( n );
    }
    return v.size();
}


//function to print the topology from the input file
void import_matrix_from_txt_file(const char* filename_X, vector <double>& v, int& rows, int& cols){

    ifstream file_X;
    string line;

    file_X.open(filename_X);
    if (file_X.is_open())
    {
        int i=0;
        getline(file_X, line);


        cols =ReadNumbers( line, v );



        for ( i=1;i<32767;i++){
            if ( getline(file_X, line) == 0 ) break;
            ReadNumbers( line, v );

        }

        rows=i;

        if(rows >32766) cout<< "N must be smaller than MAX_INT";

        file_X.close();
    }
    else{
        cout << "file open failed";
    }


    size = rows;
    //dynamic allocation of graph with number of rows and columns
    graph = new long*[rows];

    for(int i = 0; i < rows; ++i)
        graph[i] = new long[cols];
    //assign values from the vector holding the values read from the file to the graph and print the topology matrix
    for (int i=0;i<rows;i++){
        for (int j=0;j<cols;j++){
            graph[i][j] = v[i*cols+j];
            cout<<graph[i][j]<<"\t";
        }
        cout << endl;
    }
}


//function to implement Dijkstra algorithm
void dijkstra(long **graph,int n,int startnode)
{


    long visited[size],count,mindistance,nextnode,i,j;
    //dynamic allocation of arrays declared above
    interfc = new long [size];
    dist = new long [size];
    distanc = new long [size];
    pred = new long [size];
    cost = new long*[size];
    for(int i = 0; i < size; ++i)
        cost[i] = new long[size];
    path = new long*[size];
    for(int i = 0; i < size; ++i)
        path[i] = new long[size];

    //pred[] stores the predecessor of each node
    //count gives the number of nodes seen so far
    //create the cost matrix
    for(i=0;i<n;i++)
        for(j=0;j<n;j++)
            if(graph[i][j]==0 || graph[i][j]==-1)
                cost[i][j]=INFINITY;
            else
                cost[i][j]=graph[i][j];

    //initialize pred[],distance[] and visited[]
    //create the cost matrix

    for(i=0;i<n;i++)
    {
        distanc[i]=cost[startnode][i];
        pred[i]=startnode;
        visited[i]=0;
    }

    distanc[startnode]=0;
    visited[startnode]=1;
    count=1;

    while(count<n-1)
    {
        mindistance=INFINITY;

        //nextnode gives the node at minimum distance
        for(i=0;i<n;i++)
            if(distanc[i]<mindistance&&!visited[i])
            {
                mindistance=distanc[i];
                nextnode=i;
            }

            //check if a better path exists through nextnode
            visited[nextnode]=1;
            for(i=0;i<n;i++)
                if(!visited[i])
                    if(mindistance+cost[nextnode][i]<distanc[i])
                    {
                        distanc[i]=mindistance+cost[nextnode][i];
                        pred[i]=nextnode;
                    }
        count++;
    }
    interfc[startnode] = 0;
    for(int y = 0;y<size;y++)
        path[startnode][y] = 0;

    for(i=0;i<n;i++)
        if(i!=startnode)
        {

            //dist[i] = distanc[i];


            j=i;

            int temp = i;
            while(j!=startnode)
            {
                j=pred[j];

                if(j!=startnode)
                    temp = j;
            }
             interfc[i] = temp;

        }

}
//main function prompts the master menu until choice 6 is selected
int main()
{
    long rows, columns;

    check = new int[size];
    int choice;

    do
    {


        cout << endl
        << " (1) CREATE A NETWORK TOPOLOGY\n"
        << " (2) BUILD A FORWARD TABLE\n"
        << " (3) SHORTEST PATH TO DESTINATION ROUTE\n"
        << " (4) MODIFY A TOPOLOGY(CHANGE THE STATUS OF THE ROUTER\n"
        << " (5) BEST ROUTER FOR BROADCAST\n"
        << " (6) EXIT\n"
        << "  COMMAND:\t";
        cin >> choice;

        switch (choice)
        {
            case 1:
                    //open and read the topology file and print the topology
                    {

                     vector <double> v;
                     //user input file name
                    cout << "Enter file name: ";
                    getchar();
                    getline(cin, fileName);
                    int rows=0;
                    int cols=0;
                    cout<<"\nReview Original topology matrix: "<<endl;
                    //print the topology matrix
                    import_matrix_from_txt_file(fileName.c_str(),v,rows,cols);

                    }
                    break;
            //user input source router and its connection table
            case 2:

            {
                //code to take user input and pass this as a parameter to dijksra function to find the interface for this router to reach all the routers
                cout<<"Enter Source: ";

                cin>>src;
                if(src>size)
                    cout<<"\nInvalid Source";
                else
                {
                     cout<<"\nRouter"<<src<<" connection table"<<endl<<"Destination\tInterface\n===================================\n";
                    dijkstra(graph,size, src-1);
                    for(int i = 0;i<size;i++)
                    {

                        if(i == src-1)
                            cout<<src<<"\t"<<"-"<<endl;
                        else
                            cout<<i+1<<"\t"<<interfc[i]+1<<endl;
                    }


                }


            }

            break;

            case 3:

            //user input destination router and its shortest distance
            {
                long path[INFINITY];
                int k =0;
                //user inputs the destination router and distanc[] array holds the shortest distance for every destination so distnc[dest-1] has the shortest distance also pred[] holds the next router to reach to the destination
                cout<<"Enter Destination: ";
                cin>>dest;
                if(dest == src || dest>size)
                {
                    cout<<"\nInvalid Destination";
                }
                else{
                        int j;
                cout<<"\nShortest path from Router"<<src<<" to Router"<<dest<<" is ";
                for(int i=0;i<size;i++)
                    if(i!=src && i == dest-1)
                    {
                        path[k] = i+1;
                        k++;


                        j=i;

                        do
                        {
                            j=pred[j];

                            path[k] = j+1;
                            k++;
                        }while(j!= src-1);

                     }
                     k--;
                for( ; k>=0;k--)
                {
                    if(k==0)
                        cout<<path[k];
                    else
                        cout<<path[k]<<"->";

                }

                cout<<", the cost is "<<distanc[dest-1];

                }


            }
            break;
            //remove a router from topology and update the connection table and shortes path or the destination router
            case 4:
            {
                //if directly user selects choice four after choice two from the menu it will prompt to enter the destination router

                if(dest == -10)
                {
                    cout<<"\nDestination Router not selected! Enter now:";
                    cin>>dest;

                }
                if(dest == src || dest>size )
                {
                    cout<<"\nInvalid Choice";
                    break;
                }


                cout<<"\ndelete A Router from this Topology\n"<<"Down Router?";
                cin>>dn;

                if(dn == src || dn == dest || dn>size || check[dn-1] == -1)
                {
                     cout<<"\nInvalid Choice";
                     break;

                }
                check[dn-1] = -1;

                //update the destination router's row and columnn by -1
                for(int i = 0;i<size; i++)
                {
                    graph[dn-1][i] =-1;
                    graph[i][dn-1] = -1;
                }




                cout<<"\nUpdated Router"<<src<<" connection table"<<endl<<"Destination\tInterface\n===================================\n";
                dijkstra(graph,size, src-1);
                for(int i = 0;i<size;i++)
                {

                    if(i == src-1 || i == dn-1 || check[i] == -1)
                        cout<<i+1<<"\t"<<"-"<<endl;
                    else
                        cout<<i+1<<"\t"<<interfc[i]+1<<endl;
                }
                long path[INFINITY];
                int k =0;

                int j;
                cout<<"\nUpdated Shortest path from Router"<<src<<" to Router"<<dest<<" is ";
                for(int i=0;i<size;i++)
                    if(i!=src && i == dest-1)
                    {

                        path[k] = i+1;
                        k++;
                        dist[i] = distanc[i];

                        j=i;

                        do
                        {
                            j=pred[j];

                            path[k] = j+1;
                            k++;


                        }while(j!= src-1);

                     }
                     k--;
                for( ; k>=0;k--)
                {
                    if(k==0)
                        cout<<path[k];
                    else
                        cout<<path[k]<<"->";

                }

                cout<<", updated the cost is "<<distanc[dest-1];


            }
            break;
            //router which has the lowest cost to reach all other routers in the topology
            case 5:
            {
                  long min_cost[size];
                  long m = 0;
                  long low_cost = 0;
                  long costi = 0;
                  for(int i = 0;i<size;i++)
                  {
                        costi = 0;

                        dijkstra(graph,size, i);
                        for(int j=0;j<size;j++)
                        {

                            if(j!=i && j != dn-1)
                            {
                                costi = costi+distanc[j];
                            }

                        }
                        min_cost[m] = costi;
                        m++;
                  }

                  cout<<"Router";
                  low_cost = *std::min_element(min_cost,min_cost+size);
                  for(int i = 0; i<size; i++)
                  {
                      if(low_cost == min_cost[i])
                      {
                          cout<<i+1;
                          break;
                      }


                  }

                  cout<<" has the shortest path to all other routers with cost "<<low_cost;
            }

            break;

            //exit from program
            case 6:


                {
                    cout<<"\nExit CS542-04 2017 Fall project. Good Bye!";
                    return 0;
                }
                break;
            //default case
            default:
                cout << "Not a Valid Choice. \n"
                    << "Choose again.\n";
            break;
        }

    }while (choice !=7);

    for(int i = 0; i < size; ++i) {
        delete[] graph[i];
    }
    //Free the array  pointers
    delete[] graph;
    delete[] pred;
    delete[] dist;
    delete[] interfc;
    delete[] distanc;

     for(int i = 0; i < size; ++i) {
        delete[] cost[i];
    }
    delete[] cost;
    path = new long*[size];
     for(int i = 0; i < size; ++i) {
        delete[] path[i];
    }
    delete[] path;
    return 0;
}

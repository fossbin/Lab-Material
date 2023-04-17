#include <iostream>
#include <limits.h>
using namespace std;

class transportation
{
    int s, d;
    int *supply, *demand, **alloc, **cost;

public:
    transportation() {}
    transportation(int s, int d, int *supply, int *demand, int **cost, int **alloc)
    {
        this->s = s;
        this->d = d;
        this->supply = supply;
        this->demand = demand;
        this->alloc = alloc;
        this->cost = cost;
    }
    void getLeastCost()
    {
        while (true)
        {
            int r = -1, c = -1, minCost = INT_MAX;
            for (int i = 0; i < s; i++)
                for (int j = 0; j < d; j++)
                    if (supply[i] > 0 && demand[j] > 0 && cost[i][j] < minCost)
                    {
                        r = i;
                        c = j;
                        minCost = cost[i][j];
                    }
            // If all cells have been assigned, we're done
            if (r == -1)
                break;

            // Assign as much as possible to the minimum cost cell
            int quantity = min(supply[r], demand[c]);
            alloc[r][c] = quantity;
            supply[r] -= quantity;
            demand[c] -= quantity;
        }
    }
    void getNorthWest()
    {
        int i = 0, j = 0;
        while (i < s && j < d)
        {
            int quantity = min(supply[i], demand[j]);
            alloc[i][j] = quantity;
            supply[i] -= quantity;
            demand[j] -= quantity;
            if (supply[i] == 0)
                i++;
            else if (demand[j] == 0)
                j++;
        }
    }
    int calcFeasibleSol()
    {
        int totalCost = 0;
        for (int i = 0; i < s; i++)
            for (int j = 0; j < d; j++)
            {
                if (alloc[i][j] == 0)
                    continue;
                totalCost += alloc[i][j] * cost[i][j];
            }
        return totalCost;
    }
    void displayFeasibleSol()
    {
        cout << "\nInitial feasible solution:" << endl;
        for (int i = 0; i < s; i++)
        {
            for (int j = 0; j < d; j++)
                cout << alloc[i][j] << "\t";
            cout << endl;
        }
        cout << "Total cost: " << calcFeasibleSol() << endl;
    }
    ~transportation()
    {
        delete[] supply;
        delete[] demand;
        delete[] cost;
        delete[] alloc;
    }
};

int main()
{
    int ch, i, j, k=0, s, d;
    char reply;
    do
    {
        cout << "\nENTER THE PROBLEM\n";
        cout << "Enter the number of sources: ";
        cin >> s;
        cout << "Enter the number of destinations: ";
        cin >> d;
        int *supply = new int[s];
        int *demand = new int[d];

        // Input the capacity of each source
        for (i = 0; i < s; i++)
        {
            cout << "Enter the capacity of source " << i + 1 << ": ";
            cin >> supply[i];
        }

        // Input the demand of each destination
        for (j = 0; j < d; j++)
        {
            cout << "Enter the demand of destination " << j + 1 << ": ";
            cin >> demand[j];
        }

        int sums = 0, sumd = 0;
        for (i = 0; i < s; i++)
            sums += supply[i];
        for (i = 0; i < d; i++)
            sumd += demand[i];
        
        if(sums<sumd)
        {
            int *temp=new int[s+1];
            for(i=0;i<s;i++)
                temp[i]=supply[i];
            temp[i]=sumd-sums;
            delete[] supply;
            supply=temp;
            k=1;
        }
        else if(sumd<sums)
        {
            int *temp = new int[d + 1];
            for (i = 0; i < d; i++)
                temp[i] = demand[i];
            temp[i] = sums-sumd;
            delete[] demand;
            demand = temp;
            k=-1;
        }

        // Input the cost matrix
        int newS=s,newD=d;
        if(k==1)
            newS++;
        else if(k==-1)
            newD++;
        int **cost = new int *[newS];
        int **alloc = new int *[newS];
        for (i = 0; i < newS; i++)
        {
            cost[i] = new int[newD];
            alloc[i] = new int[newD];
        }
        for (int i = 0; i < s; i++)
        {
            for (int j = 0; j < d; j++)
            {
                alloc[i][j] = 0; 
                cout<<"Enter the cost of shipping from source " << i + 1 << " to destination " << j + 1 << ": ";
                cin >> cost[i][j];
            }   
        }
        if(k==1)
            for(i=0;i<d;i++)
                alloc[s][i]=cost[s][i]=0;
        else if(k==-1)
            for (i = 0; i < s; i++)
                alloc[i][d] = cost[i][d] = 0;
        s=newS;d=newD;
        transportation obj(s, d, supply, demand, cost, alloc);

        cout << "\nMENU\n1.North-West Method\t2.Least-Cost Method\t3.Exit";
        cout << "\nEnter your choice:";
        cin >> ch;
        switch (ch)
        {
        case 1:
            obj.getNorthWest();
            obj.displayFeasibleSol();
            break;

        case 2:
            obj.getLeastCost();
            obj.displayFeasibleSol();
            break;
        case 3:
            exit(0);
            break;
        default:
            cout << "Invalid input";
        }
        cout << "\nContinue? : (Y?N): ";
        cin >> reply;
        if (reply == 'n' || reply == 'N')
        {
            obj.~transportation();
            delete supply, demand, alloc, cost;
        }
    } while (reply == 'y' || reply == 'Y');
}

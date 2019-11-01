#include <iostream>
#include<bits/stdc++.h>
using namespace std;

#define DT pair<int, state*>

//global variable to count the nodes expanded in the ModifiedExpand function
int no_of_expanded;

class state
{
    vector<vector<int> >data;           //Board Arrangement
    vector<vector<int> >goal_data;      //Goal Board Arrangement
    
    int g;  //Distance from initial state
    int h;  //Heuristic distance to goal state
    pair<int, int> zeroPos; //Location of the Blank

public:

    //getters and setters
    vector<vector<int> >getData()
    {
        return data;
    }

    void setg(int x)
    {
        this->g=x;
    }

    int getg()
    {
        return this->g;
    }

    int geth()
    {
        return this->h;
    }

    vector<vector<int> > getgoal_data()
    {   
        return goal_data;
    }
    
    //  Heuristic #1
    int TileMismatch()
    {
        int diff=0;
        vector<vector<int> >data1;
        vector<vector<int> >data2;

        data1=this->getData();
        data2=this->goal_data;

        for(int i=0; i<3; i++)
        {
            for(int j=0; j<3; j++)
            {
                if(data1[i][j]!=0)
                {
                    if(data1[i][j]!=data2[i][j])
                        diff++;
                }
            }
        }

        return diff;
    }

    //Heuristic #2
    int ManhattanDist()
    {
        int dist=0;
        vector<vector<int> >data1, data2;

        data1=this->getData();
        data2=goal_data;

        map<int, pair<int, int> > m;
        for(int i=0; i<3; i++)
        {
            for(int j=0; j<3; j++)
            {
                m[data2[i][j]]=make_pair(i, j);
            }
        }

        for(int i=0; i<3; i++)
        {
            for(int j=0; j<3; j++)
            {
                if(data1[i][j]!=0)
                {
                    if(data1[i][j]!=data2[i][j])
                    {
                        pair<int, int> g=m[data1[i][j]];
                        dist+=abs(g.first-i)+abs(g.second-j); //No. of non-diagonal steps
                    }
                }
            }
        }
        return dist;
    }

    //Constructor
    state(vector<vector<int> >vals, int heuristic) 
    {

        for(int i=0; i<vals.size(); i++)
        {
            for(int j=0; j<vals[i].size(); j++)
            {
                if(vals[i][j]==0)
                    zeroPos=make_pair(i, j); //Blank's location
            }
        }

        this->data=vals;

        vector<int>v1;
        vector<vector<int> >v;

        int c=0;
        for(int i=0; i<3; i++)
        {
            v1.clear();
            
            for(int j=0; j<3; j++)
            {
                v1.push_back(c);
                c++;
            }
            
            v.push_back(v1);
        }
        goal_data = v;  //goal_data={{0, 1, 2}, {3, 4, 5}, {6, 7, 8}}
        g=INT_MAX;

        if(heuristic==1)
            h=this->TileMismatch();
        else
            h=this->ManhattanDist();
    }


    bool equals(state* s2)
    {
        bool flag=true;                                                                                                                                                                                                                                                                                                                                                                                                                         
        vector<vector<int> >data2=s2->getData();

        for(int i=0; i<3; i++)
        {
            for(int j=0; j<3; j++)
            {
                if(data[i][j]!=data2[i][j])
                {
                    flag=false;
                }
            }
        }
        return flag;
    }

    pair<int, int> getBlank()
    {
        return zeroPos;
    }

    void print()
    {
        for(int i=0; i<3; i++)
        {
            for(int j=0; j<3; j++)
            {
                cout<<data[i][j]<<" ";
            }
            cout<<endl;
        }
    }

    //Adjacent states
    vector<state*> children(int heuristic)
    {
        vector<state*> c;
        int x=getBlank().first;
        int y=getBlank().second;
        vector<vector<int> >num=getData();

        if(x>0)
        {
            swap(num[x][y], num[x-1][y]);
            c.push_back(new state(num, heuristic));
            swap(num[x][y], num[x-1][y]);
        }

        if(x<2)
        {
            swap(num[x][y], num[x+1][y]);
            c.push_back(new state(num, heuristic));
            swap(num[x][y], num[x+1][y]);
        }


        if(y>0)
        {
            swap(num[x][y], num[x][y-1]);
            c.push_back(new state(num, heuristic));
            swap(num[x][y], num[x][y-1]);
        }

        if(y<2)
        {
            swap(num[x][y], num[x][y+1]);
            c.push_back(new state(num, heuristic));
            swap(num[x][y], num[x][y+1]);
        }

    return c;
    }

    queue<state*> ModifiedExpand(int depth, int heuristic)
    {
        int flag=0;
        queue<state*>q;
        queue<state*> expanded_states;
        q.push(this);   //For bfs traversal
    
        int root_distance = this->g; 
        while(!q.empty())
        {
            state* root=q.front();
            q.pop();
            vector<state*> temp=root->children(heuristic);
            for(int i=0; i<temp.size(); i++)
            {
                temp[i]->g=root->g+1;
                if(temp[i]->g - root_distance > depth)
                    continue;
                expanded_states.push(temp[i]);  //For all nodes expanded

                no_of_expanded++;

                q.push(temp[i]);
            }

        }
        
        return expanded_states;
    }

    void customPrint()
    {
        cout<<"\n";
        cout<<"The Popped state is:\n ";
        this->print();
        cout<<"g value: "<<g<<" ";
        cout<<"h value: "<<h<<" ";
    }
};


int AStar(state* start, int depth, int heuristic)
{
    int max_fringe_length=0;

    set<vector<vector<int> > > explored;    //To avoid cycles
    start->setg(0);
    state* goal=new state(start->getgoal_data(), 1);
    priority_queue <DT, vector<DT>, greater<DT> > pq;   //To explore nodes in descending order of f
    pq.push(make_pair(0, start));

    if(start->equals(goal))
    {
        return 0;
    }

    while(!pq.empty())
    {

        DT topp=pq.top();
        pq.pop();
        state* u=topp.second;

        if(u->equals(goal)) //Checking for goal state when popping
        {
            cout<<"\nThe maximum number of elements in the fringe is: "<<max_fringe_length;
            return u->getg();
        }

        //Using the state's data as the hashing function
        explored.insert(u->getData());
        queue<state*> q2=u->ModifiedExpand(depth, heuristic);
        
        int __cnt = 0;

        while(!q2.empty())
        {
            __cnt++;
            state* q2i=q2.front();
            q2.pop();
            if(explored.find(q2i->getData())==explored.end())
            {
                pq.push(make_pair(q2i->getg()+q2i->geth(), q2i));
                max_fringe_length=max(max_fringe_length, (int)pq.size());
            }

        }
    }
}

//For input
vector<vector<int> > RandomState()
{
    vector<vector<int> > goal_data;
    vector<int>v1;

    int zerox=0, zeroy=0;

    int c=0;
    for(int i=0; i<3; i++)
    {
        v1.clear();
    
        for(int j=0; j<3; j++)
        {
            v1.push_back(c);
            c++;
        }
    
        goal_data.push_back(v1);
    }

    for(int i=0; i<40; i++)         //Worst case is 31 moves away
    {
        int randNum = (rand() % 4); //For 4 possible values
        
        if(randNum==0 && zerox>0)
        {
            swap(goal_data[zerox][zeroy], goal_data[zerox-1][zeroy]);
            zerox--;
        }

        if(randNum==1 && zerox<2)
        {
            swap(goal_data[zerox][zeroy], goal_data[zerox+1][zeroy]);
            zerox++;
        }


        if(randNum==2 && zeroy>0)
        {
            swap(goal_data[zerox][zeroy], goal_data[zerox][zeroy-1]);
            zeroy--;
        }

        if(randNum==3 && zeroy<2)
        {
            swap(goal_data[zerox][zeroy], goal_data[zerox][zeroy+1]);
            zeroy++;
        }
    }

    return goal_data;
    
}

pair< pair<int, int>, double> puzzle_solver(vector<vector<int> >v, int depth, int heuristic)
{
    no_of_expanded=0;
    state* start= new state(v, heuristic);
    
    if(heuristic==1)
        cout<<"\nDepth: "<<depth<<", Heuristic: TileMismatch";
    else
        cout<<"\nDepth: "<<depth<<", Heuristic: ManhattanDist";
    
    struct timespec starttime,endtime;
    clock_gettime(CLOCK_MONOTONIC, &starttime);
    int no_of_moves=AStar(start, depth, heuristic);
    cout<<"\nThe number of moves is: "<<no_of_moves;
    cout<<"\nThe number of nodes expanded is: "<<no_of_expanded;
    clock_gettime(CLOCK_MONOTONIC, &endtime);
    double timetaken=(endtime.tv_sec - starttime.tv_sec)*1e9;
    timetaken = (timetaken + (endtime.tv_nsec-starttime.tv_nsec))*1e-9;
    cout<<"\nTime taken : "<<fixed<<timetaken<<setprecision(9)<<endl<<endl;

    pair<int, int>tmp=make_pair(no_of_moves,no_of_expanded );
    pair< pair<int, int>, double> ans=make_pair(tmp, timetaken);

    return ans;
}

int main() 
{
    srand(time(0));
    for(int i=0; i<10; i++)
    {
        cout<<"-----------RANDOM STATE NUMBER "<<i+1<<" -----------\n";
        vector<vector<int> >v=RandomState();

        cout<<"The initial state is: \n";
        for(int i=0; i<3; i++)
        {
            for(int j=0; j<3; j++)
            {
                cout<<v[i][j]<<" ";
            }
            cout<<endl;
        }
        cout<<endl;

        int depth;
        cout<<"Enter the depth\n";
        cin>>depth;

        pair< pair<int, int>, double> ans1, ans2, ans3, ans4;
        ans1=puzzle_solver(v, 1, 1);
        ans2=puzzle_solver(v, 1, 2);
        ans3=puzzle_solver(v, depth, 1);
        ans4=puzzle_solver(v, depth, 2);
        

        int max_moves=max(ans1.first.first, ans2.first.first);
        max_moves=max(max_moves, ans3.first.first);
        max_moves=max(max_moves, ans4.first.first);

        int max_nodes_expanded=max(ans1.first.second, ans2.first.second);
        max_nodes_expanded=max(max_nodes_expanded, ans3.first.second);
        max_nodes_expanded=max(max_nodes_expanded, ans4.first.second);

        double max_time_taken=max(ans1.second, ans2.second);
        max_time_taken=max(max_time_taken, ans3.second);
        max_time_taken=max(max_time_taken, ans4.second);

        int min_moves=min(ans1.first.first, ans2.first.first);
        min_moves=min(min_moves, ans3.first.first);
        min_moves=min(min_moves, ans4.first.first);

        int min_nodes_expanded=min(ans1.first.second, ans2.first.second);
        min_nodes_expanded=min(min_nodes_expanded, ans3.first.second);
        min_nodes_expanded=min(min_nodes_expanded, ans4.first.second);

        double min_time_taken=min(ans1.second, ans2.second);
        min_time_taken=min(min_time_taken, ans3.second);
        min_time_taken=min(min_time_taken, ans4.second);

            
        if(min_nodes_expanded==ans1.first.second)
            cout<<"\nMinimum nodes expanded by depth 1, TileMismatch";
        else if(min_nodes_expanded==ans2.first.second)
            cout<<"\nMinimum nodes expanded by depth 1, ManhattanDist";
        else if(min_nodes_expanded==ans3.first.second)
            cout<<"\nMinimum nodes expanded by depth "<<depth<<", TileMismatch";
        else if(min_nodes_expanded==ans4.first.second)
            cout<<"\nMinimum nodes expanded by depth "<<depth<<", ManhattanDist";
        cout<<endl;

        if(max_nodes_expanded==ans1.first.second)
            cout<<"\nMaximum nodes expanded by depth 1, TileMismatch";
        else if(max_nodes_expanded==ans2.first.second)
            cout<<"\nMaximum nodes expanded by depth 1, ManhattanDist";
        else if(max_nodes_expanded==ans3.first.second)
            cout<<"\nMaximum nodes expanded by depth "<<depth<<", TileMismatch";
        else if(max_nodes_expanded==ans4.first.second)
            cout<<"\nMaximum nodes expanded by depth "<<depth<<", ManhattanDist";
        cout<<endl;


        if(min_time_taken==ans1.second)
            cout<<"\nMinimum time taken by depth 1, TileMismatch";
        else if(min_time_taken==ans2.second)
            cout<<"\nMinimum time taken by depth 1, ManhattanDist";
        else if(min_time_taken==ans3.second)
            cout<<"\nMinimum time taken by depth "<<depth<<", TileMismatch";
        else if(min_time_taken==ans4.second)
            cout<<"\nMinimum time taken by depth "<<depth<<", ManhattanDist";
        cout<<endl;

        if(max_time_taken==ans1.second)
            cout<<"\nMaximum time taken by depth 1, TileMismatch";
        else if(max_time_taken==ans2.second)
            cout<<"\nMaximum time taken by depth 1, ManhattanDist";
        else if(max_time_taken==ans3.second)
            cout<<"\nMaximum time taken by depth "<<depth<<", TileMismatch";
        else if(max_time_taken==ans4.second)
            cout<<"\nMaximum time taken by depth "<<depth<<", ManhattanDist";
        cout<<endl;
    }

    cout<<"\nSoundarya Krishnan, 2016B5A70472G";
    cout<<"\n\nAs we can see, among the depth, the least depth always takes lesser time, as well as space (nodes expanded). This is because when the depth increases, we unnecessarily expand more nodes. Also, among the heuristics, Manhattan Distance takes much lesser time and space complexity as it always dominates TileMismatch, so lesser nodes need to be explored.\n";

    cout<<"\n\n-----------Thank you for playing-----------\n";
    
    
}
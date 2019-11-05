#include <iostream>
#include<string>
#include<bits/stdc++.h>
#include<stdlib.h>
using namespace std;

string removeSpaces(string str)
{
    str.erase(remove(str.begin(), str.end(), ' '), str.end());
    return str;
}


struct Node{
    int attribute_no;
    vector<struct Node*> atr_type;
    map<string,int> next;

};

vector<vector<string> > adj(32600),test(17000); //learn 2D vector is (global)
vector<vector<string> > vals_attr_vec(20); //This contains attributes
vector<int> memo;
long int correct=0,wrong=0;
struct Node* check;
double t_acc=0;
long int runs=0;
double recall=0,precision=0;
long int trp=0,trn=0,flp=0,fln=0;
void create_memo()
{
    int j=0,more;
    for(j=32560;j>=2;j--)
    {
        if(adj[j][14]!="<=50K")
        {
            more++;
            memo[j] = more;
        }
        memo[j] = more;
    }
}                              //This stores the no of elements which have >50k at index i;

class CompareDist										//compare function for priority_queue
{
public:
    bool operator()(pair<string,int> n1,pair<string,int> n2) {
        return n1.second>n2.second;
    }
};

int cont_to_bool_vals()                     //After this only 1&0 will remain in place of data in contionuous attributes.
{
    int i=0,j=0;
	pair<string,int> prev ,curr;
	vector<double> points;
	priority_queue <pair<string,int>,vector<pair<string,int> >,CompareDist> pq;
	for (int i = 0; i < 14; ++i)
	{
		if (i==0||i==2||i==4||i==10||i==11||i==12)
		{
			for(j=1;j<32560;j++)
            {
                pq.push(make_pair(adj[j][14],atoi(adj[j][i].c_str())));
            }
            prev = pq.top();
            pq.pop();
            while(!pq.empty())
            {
                curr = pq.top();
                pq.pop();
                if(prev.first!=curr.first)
                {
                    points.push_back((prev.second+curr.second)/2);
                }

                prev  = curr;
            }
            vector<double> ::iterator it;
            int score=0,sc_index,mx=0,k=0;
            //cout<<points.size()<<endl;
            for(it=points.begin(),k=0;it!=points.end();++it)
            {
                k++;
                //cout<<k<<endl;
                score=0;
                for(j=1;j<32560;j++)
                {
                    if(*it<atoi(adj[j][i].c_str()))
                    {
                        if(adj[j][14]==" >50K")
                            score++;
                    }
                    else if(adj[j][14]==" <=50K")
                        score++;

                }
                if(score>mx)
                {
                    mx = score;
                    sc_index = k;
                }

            }

            for(j=1;j<32560;j++)
            {
                if(points[sc_index]> atoi(adj[j][i].c_str()))
                    adj[j][i] = "1";
                else
                    adj[j][i] = "0";

            }
            points.clear();
            //cout<<"After for loop";
		}
	}
	return 0;
}



int cont_to_bool_vals2()                     //After this only 1&0 will remain in place of data in contionuous attributes.
{
    int i=0,j=0;
	pair<string,int> prev ,curr;
	vector<double> points;
	priority_queue <pair<string,int>,vector<pair<string,int> >,CompareDist> pq;
	for (int i = 0; i < 14; ++i)
	{
		if (i==0||i==2||i==4||i==10||i==11||i==12)
		{
			for(j=1;j<16281;j++)
            {
                pq.push(make_pair(test[j][14],atoi(test[j][i].c_str())));
            }
            prev = pq.top();
            pq.pop();
            while(!pq.empty())
            {
                curr = pq.top();
                pq.pop();
                if(prev.first!=curr.first)
                {
                    points.push_back((prev.second+curr.second)/2);
                    //cout<<prev.first<<"   ->"<<curr.first<<endl;

                }

                prev  = curr;
            }
            vector<double> ::iterator it;
            int score=0,sc_index,mx=0,k=0;
            //cout<<points.size()<<endl;
            for(it=points.begin();it!=points.end();++it)
            {
                k++;

                score=0;
                for(j=1;j<16281;j++)
                {
                    if(*it<atoi(test[j][i].c_str()))
                    {
                        if(test[j][14]==" >50K.")
                            score++;
                    }
                    else if(test[j][14]==" <=50K.")
                        score++;

                }
                if(score>mx)
                {
                    mx = score;
                    sc_index = k;
                }



            }
            for(j=1;j<16281;j++)
            {
                if(points[sc_index] > atoi(test[j][i].c_str()))
                    test[j][i] = "1";
                else
                    test[j][i] = "0";

            }points.clear();
		}
	}
	return 0;
}
//----------------------------------------------------------------------------------------------------------------------------------
//finding entropy

double find_entropy(double lesser, double more)
{
    if(lesser==0)
        return 1;
    else if(more==0)
        return 0;
    else
    return -(lesser/(lesser+more))*log2(lesser/(lesser+more)) - (more/(lesser+more))*log2(more/(lesser+more));
}

double find_entropy1(int attr_no , string attr, vector<int> index)
{
    //cout<<"in find ent"<<endl;
	double lesser=0, more=0;
	double total=0;
	int i;
	for(i=1;i<index.size();i++)
	{
		if(adj[index[i]][attr_no]==attr)
		{
			total++;
			//cout<<adj[i][14]<<endl;
			if(adj[index[i]][14]==" <=50K")
			{
				lesser++;
			}
			else
			{
				more++;
			}
		}
	}
	//cout<<"greater->"<<more;
    //cout<<"lesser->"<<lesser<<endl;
    //cout<<"total="<<total;
	return (total/index.size())*find_entropy(lesser,more);
}

double info_gain(int att_no,vector<int> &index,double l,double m){
    int i;
    double sum=0;
    //cout<<"att="<<att_no<<"size="<<vals_attr_vec[att_no].size()<<endl;
    for(i=0;i<vals_attr_vec[att_no].size();i++){
        sum+=find_entropy1(att_no,vals_attr_vec[att_no][i],index);

    }
    //cout<<2<<endl;
    double ent=0;
    //cout<<3<<endl;
    ent=find_entropy(l,m);
    //cout<<att_no<<"   "<<ent<<"   "<<sum<<endl;
    return ent-sum;
}

//----------------------------------------------------------------------------------------------------------------------------------------
//finding missing values

void find_missing_values()
{
	int i=0;
	for (int i = 0; i < 14; ++i)
	{
		if (i==0||i==2||i==4||i==10||i==11||i==12)
		{
			int j;
			double sum=0,count=0;
			for (j = 1; j <32560; ++j)
			{
				if(adj[j][i]!=" ?")
				{
					sum+= atof(adj[j][i].c_str());
					count++;
				}
			}
			double avg = sum/count;
			for (j = 1; j <32560; ++j)
			{
				if(adj[j][i]==" ?")
				{
				    stringstream s;
				    s<<avg;
					adj[j][i] = s.str();    //<- might be wrong
				}
			}
		}
		else
		{
			std::map<string, int> map1;
			list <int> missed;
			int j=0;
			for(j=1;j<32560;j++)
			{
				if(adj[j][i]!=" ?")
				{
					map1[adj[j][i]] = map1[adj[j][i]]?(map1[adj[j][i]]+1):1;    //unknown values are being replace by mode of that column(attribute)
				}
				else{
                    missed.push_back(j);
				}
			}
			string result;
			int max = 0;
			std::map<string, int> ::iterator itr;
			for(itr=map1.begin();itr!=map1.end();itr++)
			{
				if(itr->second > max)
				{
					max = itr->second;
					result = itr->first;
					//missed.push_back(itr->second);
					//cout<<result<<endl;
				}
			}
			list<int> ::iterator it;
			for(it=missed.begin();it!=missed.end();++it)
			{
				adj[*it][i] = result;                        //unknown values are being replace by value with highest frequency
				//cout<<result<<endl;
			}
            map1.clear();
            missed.clear();
		}
	}

}



void find_missing_values2()
{
	int i=0;
	for (int i = 0; i < 14; ++i)
	{
		if (i==0||i==2||i==4||i==10||i==11||i==12)
		{
			int j;
			double sum=0,count=0;
			for (j = 1; j <16281; ++j)
			{
				if(test[j][i]!=" ?")
				{
					sum+= atof(test[j][i].c_str());
					count++;
				}
			}
			double avg = sum/count;
			for (j = 1; j <16281; ++j)
			{
				if(test[j][i]==" ?")
				{
				    stringstream s;
				    s<<avg;
					test[j][i] = s.str();    //<- might be wrong
				}
			}
		}
		else
		{
			std::map<string, int> map1;
			list <int> missed;
			int j=0;
			for(j=1;j<16281;j++)
			{
				if(test[j][i]!=" ?")
				{
					map1[test[j][i]] = map1[test[j][i]]?(map1[test[j][i]]+1):1;    //unknown values are being replace by average of that column(attribute)
				}
				else{
                    missed.push_back(j);
				}
			}
			string result;
			int max = 0;
			std::map<string, int> ::iterator itr;
			for(itr=map1.begin();itr!=map1.end();itr++)
			{
				if(itr->second > max)
				{
					max = itr->second;
					result = itr->first;
					//missed.push_back(itr->second);
					//cout<<result<<endl;
				}
			}
			list<int> ::iterator it;
			for(it=missed.begin();it!=missed.end();++it)
			{
				test[*it][i] = result;                        //unknown values are being replace by value with highest frequency
				//cout<<result<<endl;
			}
            map1.clear();
            missed.clear();
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------------------
//main() function
struct Node* createNode(int n){
    struct Node* root=new struct Node;

    root->attribute_no=n;
    root->atr_type.clear();
    root->next.clear();
    //cout<<root->atr_type.size()<<endl;
    //cout<<root->next.size();
    /*map<string, int > :: iterator iter=root->next.begin();
    while(iter!=root->next.end())
    {
        cout<<iter->first<<"    "<<iter->second<<endl;
        iter++;
    }*/


    //cout<<"Return in root";
    runs++;
    return root;
}


struct Node* buildTree(vector<int> &index, vector<int> &att_list){

    //cout<<index.size()<<"   "<<att_list.size()<<endl;

    struct Node* root=createNode(1);

    if(index.empty())
    {
        root->attribute_no=-2;
        cout<<"index0"<<endl;
        return root;
    }




    //cout<<"IT has created the node"<<endl;
    double lesser=0,more=0;
    for(int i=0;i<index.size();i++){
        if(adj[index[i]][14]==" <=50K"){
            lesser++;
        }
        else{
            more++;
        }
    }
    //cout<<lesser<<" "<<more<<endl;
    if(lesser==0){
        root->attribute_no=-1;//-1 means +
        //root->atr_type=NULL;
        //root->next=nullptr;
        //cout<<"Lesser"<<endl;
        return root;
    }
    else if(more==0){
        root->attribute_no=-2;//-2 means -
        //root->atr_type=nullptr;
        //root->next=nullptr;
        //cout<<"More"<<endl;
        return root;
    }
    if(att_list.empty()==1)//IF number of predicting attributes are zero
    {
        if(lesser>=more)
        {
            root->attribute_no=-2;

        }
        else
        {
            root->attribute_no=-1;
        }
        return root;
    }


    double m=0;

    for(int i=0;i<att_list.size();i++){
        double ig=info_gain(att_list[i],index,lesser,more);
        //cout<<" info gain   "<<ig<<endl;
        if(m<ig){
            m=ig;
            root->attribute_no=att_list[i];
        }

    }
    //cout<<"Info gain is     "<<m<<endl;
    if(m==0)
    {
        if(lesser>=more)
        {
            root->attribute_no=-2;

        }
        else
        {
            root->attribute_no=-1;
        }
        return root;

    }
   // cout<<"Calculated information gain"<<endl;

    for(int i=0;i<vals_attr_vec[root->attribute_no].size();i++){

        root->next[vals_attr_vec[root->attribute_no][i]]=i;
        //cout<<"It has inserted in the map"<<endl;
        vector <int> index1;
        for(int j=0;j<index.size();j++){
            if(adj[index[j]][root->attribute_no]==vals_attr_vec[root->attribute_no][i]){
                index1.push_back(index[j]);
            }
        }
        if(index1.size()==0){
            root->atr_type.push_back(createNode(-2));

        }
        else{
            vector<int> att_list1;
            for(int j=0;j<att_list.size();j++){
                if(att_list[j]!=root->attribute_no){
                    att_list1.push_back(att_list[j]);
                }
            }
            root->atr_type.push_back(buildTree(index1,att_list1));
        }
    }

    return root;

}

double classify(struct Node* root)
{

    struct Node* current=root;
    long int attno=current->attribute_no;
    for(long int i=1;i<16281;i++)
    {
        //cout<<i<<endl;
       // cout<<test[i][14]<<endl;
        current=root;
        vector<string> sample=test[i];
        while(current->atr_type.empty()==0)
        {
            ++runs;
            attno=current->attribute_no;
            string attval=sample[attno];
            long int nextno=current->next[attval];
            current=current->atr_type[nextno];
        }
        //cout<<runs<<endl;
        //runs=0;
        if(test[i][14].compare(" <=50K.")==0)
        {
            if(current->attribute_no==-2)
            {
                fln++;
                correct++;
            }
            else if(current->attribute_no==-1)
            {
                flp++;
                wrong++;
            }

        }
        else if(test[i][14].compare(" >50K.")==0)
        {
            if(current->attribute_no==-2)
            {
                trn++;
                wrong++;
            }
            else if(current->attribute_no==-1)
            {
                trp++;
                correct++;
            }
        }
    }
    cout<<"Correct are  "<<correct<<endl;
    cout<<"Wrong are    "<<wrong<<endl;
    cout<<"Accuracy is "<<((double)(1.0*correct))/(correct+wrong)<<endl;
    double accuracy=((double)(1.0*correct))/(correct+wrong);
    precision=((double)(trp*1.0))/(trp+flp);
    recall=((double)(trp*1.0))/(trp+trn);
    cout<<"Precision is "<<precision<<endl;
    cout<<"Recall is "<<recall<<endl;
    correct=0;wrong=0;
    trn=0;trp=0;fln=0;flp=0;

    return accuracy;
}


double validation(struct Node* root)
{
    struct Node * current=root;
    long int attno=current->attribute_no;
    long int corr=0,wro=0;

    for(long int i=27000;i<32560;i++)
    {
       // cout<<test[i][14]<<endl;
        //cout<<i<<endl;
        vector<string> sample=adj[i];
        current=root;
        attno=current->attribute_no;
        while(current->atr_type.empty()==0)
        {
            attno=current->attribute_no;
            string attval=sample[attno];
            long int nextno=current->next[attval];
            current=current->atr_type[nextno];
        }
        //cout<<i<<endl;
        if(adj[i][14].compare(" <=50K")==0)
        {
            if(current->attribute_no==-2)
            {
                corr++;
            }
            else if(current->attribute_no==-1)
            {
                wro++;
            }

        }
        else if(adj[i][14].compare(" >50K")==0)
        {
            if(current->attribute_no==-2)
            {
                wro++;
            }
            else if(current->attribute_no==-1)
            {
                corr++;
            }
        }
    }
    //cout<<"Correct are  "<<corr<<endl;
    //cout<<"Wrong are    "<<wro<<endl;
    //cout<<"Accuracy is "<<((double)(1.0*corr))/(corr+wro);

    double accuracy=((double)(1.0*corr))/(corr+wro);
    return accuracy;


}




pair<long int ,long int > prunning(struct Node* root,vector<int> &index)
{
   // cout<<index.size()<<endl;

    struct Node* current=root;
    if(current->atr_type.empty()==1)
    {
        long int les=0,more=0;
        for(long int i=0;i<index.size();i++)
        {
            if(adj[index[i]][14]==" <=50K")
            {
                les++;
            }
            else
            {
                more++;
            }
        }
        //cout<<++runs<<endl;
        /*int atn=current->attribute_no;
        if(les>more)
        {
            current->attribute_no=-2;
        }
        else
        {
            current->attribute_no=-1;
        }
        double tct=validation(check);
        if(tct>t_acc)
        {
            t_acc=tct;
        }
        else
        {
            current->attribute_no=atn;
        }*/
        pair<long int ,long int > temp=make_pair(les,more);
        return temp;
    }

    map<string,int > :: iterator iter=current->next.begin();
    vector<pair< long int,long int > >v;
   long int mx=-1,type=0;
   for(;iter!=current->next.end();iter++)
   {
       vector<int> index1;

       for(long int i=0;i<index.size();i++)
       {
           if(iter->first.compare(adj[index[i]][current->attribute_no])==0)
           {
               index1.push_back(index[i]);
           }

       }

       pair<long int ,long int > p;
       if(index1.empty()==0)
       {
           p=prunning(current->atr_type[iter->second],index1);
       //cout<<p.first<<"     "<<p.second<<endl;
       v.push_back(p);
       }
       else
       {

       }
       /*if(mx<p.second)
       {
           mx=p.second;
           type=p.first;
       }*/


   }
   //cout<<++runs<<endl;
   vector<pair<long int ,long int > > :: iterator it=v.begin();
    int flag=0;
    long int tpos=0,tneg=0;

   while(it!=v.end())
   {
       tneg+=it->first;
       tpos+=it->second;




       it++;
   }

   /*if(flag)
   {
        //cout<<"falag =1\n";
       pair<long int ,long int > tp=make_pair(-3,index.size());
       return tp;
   }*/
   {

        int atn=current->attribute_no;
        if(tpos>tneg)
        {
            mx=tpos;
            type=-1;
        }
        else if(tneg<tpos)
        {
            mx=tneg;
            type=-2;
        }
        else
        {
            pair<long int ,long int  > tp=make_pair(tneg,tpos);
            return tp;
        }

        current->attribute_no=type;
        map<string,int> tmap(current->next.begin(),current->next.end());
        vector<struct Node*> tnode(current->atr_type.begin(),current->atr_type.end());
        current->atr_type.clear();
        current->next.clear();
        //map<string,int> :: iterator


        //cout<<"It is goinf into validation"<<endl;
        double tact=validation(check);
        /*if(index.size()==0){
            //current->attribute_no=atn;
            pair<long int ,long int  > tp=make_pair(type,index.size());
            return tp;
        }*/

        //cout<<tact<<"   "<<type<<"  "<<atn<<"   "<<index.size()<<"  "<<tpos<<"  "<<tneg<<endl;
        //cout<<"It has done validation"<<endl;
        if(t_acc<tact)
        {
            //cout<<"It has gone in if    "<<tact<<endl;
            t_acc=tact;

            pair<long int ,long int > tp=make_pair(tneg,tpos);
            return tp;
        }
        else
        {
            current->attribute_no=atn;
            map<string,int> :: iterator titer=tmap.begin();
            while(titer!=tmap.end())
            {
                current->next.insert(make_pair(titer->first,titer->second));
                titer++;
            }
            vector<struct Node*> :: iterator tvec=tnode.begin();
            while(tvec!=tnode.end())
            {
                current->atr_type.push_back(*tvec);
                tvec++;
            }

            pair<long int ,long int  > tp=make_pair(tneg,tpos);
            return tp;
        }


   }

}


long int fclassify(struct Node* root,vector<string> &instance)
{
    struct Node * current=root;
    long int attno=current->attribute_no;
    while(current->atr_type.empty()==0)
    {
        attno=current->attribute_no;
        string attval=instance[attno];
        long int nextno=current->next[attval];
        current=current->atr_type[nextno];
    }
    return current->attribute_no;
}



int main()
{

    ifstream file("adult.data");
    double more=0,lesser=0;
    string line ="";
    long int i=1;
    while(getline(file,line) )
    {
        stringstream strstr(line);

        string word="";
        while(getline(strstr,word,','))
        {
            adj[i].push_back((word));

        }
        i++;
    }

    file.close();
    int j=0;
    vector<set<string> >attr_vals;
    set <string> attr_2;
     find_missing_values();
    cont_to_bool_vals();
    for(i=0;i<15;i++)
    {
        for(j=1;j<32560;j++)
        {
            if(adj[j][i]!=" ?")
            {
                attr_2.insert(adj[j][i]);
            }
        }
        attr_vals.push_back(attr_2);
        attr_2.clear();
    }
    set<string> ::iterator it;

    for(i=0;i<15;i++)
    {
        if(i==0||i==2||i==4||i==10||i==12||i==11){
            vector<string> conti;
            conti.push_back("0");
            conti.push_back("1");
            vals_attr_vec.push_back(conti);
        }

        for(it=attr_vals[i].begin();it!=attr_vals[i].end();++it)
        {
            vals_attr_vec[i].push_back(*it);
        }
    }




    ofstream myfile;
    myfile.open ("example1.csv");
    for(i=1;i<32560;i++){
        for(j=0;j<15;j++){
            myfile<<adj[i][j];
            myfile<<",";
        }
        myfile<<"\n";
    }
    myfile.close();

    i=1;
    /*ifstream refile("example1.csv");
    string line="";
    while(getline(refile,line) )
    {
        stringstream strstr(line);
        string word="";
        adj[i].clear();
        while(getline(strstr,word,','))
        {
            adj[i].push_back((word));
        }
        i++;
    }*/




    ifstream file1("adult.test");

    line ="";
    i=1;
    while(getline(file1,line) )
    {
        stringstream strstr(line);

        string word="";
        while(getline(strstr,word,','))
        {
            test[i].push_back((word));


        }
        i++;
    }
    find_missing_values2();
    cont_to_bool_vals2();

    ofstream myfile2;
    myfile2.open ("test1.csv");
    for(i=1;i<16281;i++){
        for(j=0;j<15;j++){
            myfile2<<test[i][j];
            myfile2<<",";
        }
        myfile2<<"\n";
    }
    myfile2.close();




    vector<int> index,att_list;
    for(i=1;i<32560;i++){
        index.push_back(i);
    }

    for(i=0;i<14;i++)
    {
        att_list.push_back(i);
    }






    cout<<"It will build tree now "<<endl;
    struct Node* dtree;
    dtree=buildTree(index,att_list);
    cout<<"Size is "<<runs<<endl;
    check=dtree;
    runs=0;
    cout<<"Accuracy after overfitting\n";
    classify(dtree);
    cout<<"\n"<<runs<<endl;
    //Building tree for prunning
    index.clear();
    for(i=1;i<=27000;i++)
    {
        index.push_back(i);
    }
    check=buildTree(index,att_list);
    dtree=check;






    t_acc=validation(check);
    cout<<"\n"<<t_acc<<endl;
    runs=0;

    cout<<"gone in Pruning\n";
    prunning(check,index);

    runs=0;


    cout<<"\nAccuracy on validation data after Prunning   ";
    cout<<t_acc<<endl;
    cout<<"Accuracy on test data after prunning ";
    classify(check);

    //Random Forest Code begins
    vector<struct Node* > forest(15);
    i=1;
    for(long int j=1;j<=10;j++)
    {
        long int ti=i+3000;
        index.clear();
        while(i<=ti)
        {
            index.push_back(i);
            i++;
        }
        forest[j]=buildTree(index,att_list);
    }
    correct=0;wrong=0;//For accuracy of forest
    for(i=1;i<16281;i++)
    {
        vector<string > sample=test[i];
        long int tp=0,tn=0;
        for(long int j=1;j<=10;j++)
        {
            if(fclassify(forest[j],sample)==-1)
            {
                tp++;
            }
            else
            {
                tn++;
            }
        }
        if(tp>tn)
        {
            if(sample[14].compare(" <=50K.")==0)
            {
                wrong++;
            }
            else
            {
                correct++;
            }
        }
        else
        {
            if(sample[14].compare(" >50K.")==0)
            {
                wrong++;
            }
            else
            {
                correct++;
            }
        }
    }
    double facc=((double)(correct*1.0))/(correct+wrong);
    cout<<"Printing the accuracy of random forest    "<<facc<<endl;
    return 0;
}
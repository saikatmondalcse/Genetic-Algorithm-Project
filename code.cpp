#include<bits/stdc++.h>
using namespace std;
#define all(x) (x).begin(), (x).end()
#define pb push_back
#define ff first
#define ss second
#define sz(x) ((int)(x).size())
typedef long long ll;

#ifndef ONLINE_JUDGE
#define deb(x) cout<<#x<<" "<<x<<"\n"
#define deb2(x,y) cout<<"[ "<<#x<<"="<<x<<","<<#y<<"="<<y<<" ]\n"
#define deba(x,n) cout<<#x<<": [ ";for(int i=0;i<n;i++){cout<<x[i]<<" ";}cout<<"]\n"; 
#define deba2(x,n,m) cout<<#x<<":\n";for(int i=0;i<n;i++){for(int j=0;j<m;j++)cout<<x[i][j]<<" ";cout<<"\n";}; 
#define debe(x) cout<<#x<<":[ ";for(auto i:x){cout<<i<<" ";}cout<<"]\n"; 
#else
#define deb(x) 
#define deb2(x,y) 
#define deba(x,n) 
#define deba2(x,n,m) 
#define debe(x) 
#endif

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

int rnd(int mn,int mx)
{
  return uniform_int_distribution<int>(mn,mx)(rng);
}

struct Block
{
	int l,w,h;

	Block(int x,int y,int z)
	{
		l=max(x,y);
		w=min(x,y);
		h=z;
	}

	bool operator<(const Block &other)const
	{
		return (this->l<other.l && this->w<other.w);
	}

};


struct Tower{

	vector<Block>Blocks;
	int height=0;
	int getHeight() const{
		return height;
	}

	bool feasible(const Block &b)
	{
		if(Blocks.empty())
			return true;
		return (Blocks.back()<b);
	}

	void put(const Block &b)
	{
		Blocks.pb(b);
		height+=b.h;
	}

	bool operator<(const Tower &other)const
	{
		return this->getHeight()>other.getHeight();
	}

};


struct GA{

	vector<Block>inputBlocks;
	vector<Tower>towers;
	int populationSize;
	int maxEpoch;
	int maxHeight;
	GA(vector<Block>blocks,int p,int e){
		inputBlocks=blocks;
		populationSize=p;
		maxEpoch=e;
	}



	Tower generateTower()
	{

		int maxRandomMiss=100;
		int randomMiss=0;
		Tower T;
		while(1)
		{

			if(randomMiss>maxRandomMiss)
				break;

			int i=rnd(0,sz(inputBlocks)-1);

			if(T.feasible(inputBlocks[i]))
			{
				T.put(inputBlocks[i]);
			}
			else
				randomMiss++;
		}
		return T;
	}


	void generateInitialPopulation(){

		while(sz(towers)<populationSize)
		{
			towers.pb(generateTower());
		}
		sort(all(towers));
		maxHeight=towers.front().getHeight();
	}


	Tower crossOver(const Tower &father,const Tower &mother){

		Tower child;

		int i1=0,i2=0;
		while( (i1<sz(father.Blocks) || i2<sz(mother.Blocks)))
		{

			if(i1>=sz(father.Blocks))
			{
				if(child.feasible(mother.Blocks[i2]))
					child.put(mother.Blocks[i2]);
				i2++;
			}
			else if(i2>=sz(mother.Blocks))
			{
				if(child.feasible(father.Blocks[i1]))				
					child.put(father.Blocks[i1]);
				i1++;
			}
			else if(father.Blocks[i1]<mother.Blocks[i2])
			{
				if(child.feasible(father.Blocks[i1]))
					child.put(father.Blocks[i1]);
				i1++;
			}
			else
			{
				if(child.feasible(mother.Blocks[i2]))				
					child.put(mother.Blocks[i2]);
				i2++;
			}
		}

		return child;
	}


	int selection()
	{
		int index=0;

		// long long totalHeight=0;
		// for(auto x:towers)
		// 	totalHeight+=x.getHeight();

		// double dr=rnd(0,1e9);
		// double nr=rnd(0,dr);
		// double f=nr/dr;

		// double offset=0;
		// while(offset<f)
		// {
		// 	offset+=(1.0*towers[index].getHeight())/totalHeight;
		// 	index++;
		// 	if(index>=populationSize)
		// 		index-=populationSize;
		// }
		// return index;
		return rnd(0,populationSize-1);
	}


	Tower generateOffSpring()
	{
		int f=selection();
		int  m=selection();
		while(m==f)
			m=selection();

		return crossOver(towers[f],towers[m]);
	}


	void GA_algo()
	{
		generateInitialPopulation();
		int epoch=0;
		int prev=0;
		int repetitions=0;
		int maxRepetitions=100;
		while(epoch<maxEpoch)
		{
			assert(sz(towers)==populationSize);

			if(prev==maxHeight)
				repetitions++;

			prev=maxHeight;

			if(repetitions>maxRepetitions)
				break;

			shuffle(all(towers),rng);
			
			for(int i=0;i<populationSize;i++)
			{
				towers.push_back(generateOffSpring());
			}
			sort(all(towers));
			while(sz(towers)>populationSize)
				towers.pop_back();
			maxHeight=towers.front().getHeight();
			++epoch;
		}
	}

};



Tower best;
void bestHeight(vector<Block>Blocks,int &mx)
{
	GA ga(Blocks,1000,10000);

    ga.GA_algo();
    
    if(ga.maxHeight>mx)
    	mx=ga.maxHeight,best=ga.towers.front();
}




int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);


    int n;
    cin>>n;
    vector<Block>Blocks;
    for(int i=0;i<n;i++)
    {
    	int x,y,z;
    	cin>>x>>y>>z;
    	Blocks.pb(Block(x,y,z));
    	Blocks.pb(Block(y,z,x));
    	Blocks.pb(Block(z,x,y));
    }

    int ans=0;

    for(int i=0;i<5;i++)
    	bestHeight(Blocks,ans);

    cout<<ans<<"\n";
    // for(auto x:best.Blocks)
    // {
    // 	cout<<x.l<<" "<<x.w<<" "<<x.h<<"\n";
    // }

    return 0;
}

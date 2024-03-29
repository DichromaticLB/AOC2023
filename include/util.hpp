#ifndef DBMW_INCLUDE_UTIL_HPP_
#define DBMW_INCLUDE_UTIL_HPP_

#include<fstream>
#include<string>
#include<vector>
#include<functional>
#include<map>
#include<iostream>
#include<queue>
#include<bits/stdc++.h>

using namespace std;

struct fiterator{

	struct _iter{
		fiterator *src;

		std::string operator*() const;
		_iter& operator++();
		bool operator!=(const _iter& other) const;

	};

	fiterator(const std::string &filename,const char& separator=' ');

	_iter begin();
	_iter end();
	std::string next();

	char sep;
	std::ifstream f;
};

std::vector<std::string> tokenize(const std::string &st,std::vector<char> separators={' '},bool ignoreEmpty=false);


template<class T,bool wrap=false>
struct board{

	using row=std::vector<T>;

	uint64_t width() const{
		if(bd.size()==0)
			throw std::runtime_error("Board: can't find width of empty board");
		return bd[0].size();
	}

	uint64_t height() const{
		return bd.size();
	}

	row& operator[](uint64_t y){
		return bd[y];
	}

	struct iter{

		board* src;
		int64_t x,y;

		iter& operator++(){
			if(x<(int64_t)src->width()-1)
				x++;
			else
			{
				x=0;
				y++;
			}


			return *this;
		}

		void dowrap(){
			if(wrap){
				int64_t w=src->width();
				int64_t h=src->height();
				while(x<0)
					x+=w;
				while(x>=w)
					x-=w;
				while(y<0)
					y+=h;
				while(y>=h)
					y-=h;

			}
		}

		iter left(){
			iter res=*this;
			res.x--;
			res.dowrap();
			return res;
		}

		iter right(){
			iter res=*this;
			res.x++;
			res.dowrap();
			return res;
		}

		iter top(){
			iter res=*this;
			res.y--;
			res.dowrap();
			return res;
		}

		iter top_left(){
			iter res=*this;
			res.y--;
			res.x--;
			res.dowrap();
			return res;
		}

		iter top_right(){
			iter res=*this;
			res.y--;
			res.x++;
			res.dowrap();
			return res;
		}

		iter bot(){
			iter res=*this;
			res.y++;
			res.dowrap();
			return res;
		}

		iter bot_left(){
			iter res=*this;
			res.y++;
			res.x--;
			res.dowrap();
			return res;
		}

		iter bot_right(){
			iter res=*this;
			res.y++;
			res.x++;
			res.dowrap();
			return res;
		}

		std::vector<iter> around4(){
			return std::vector<iter>{
				top(),
				right(),
				bot(),
				left()
			};
		}

		std::vector<iter> around8(){
			return std::vector<iter>{
				top_left(),
				top(),
				top_right(),
				right(),
				bot_right(),
				bot(),
				bot_left(),
				left()

			};
		}

		bool operator<(const iter& other) const{
			if(y==other.y)
				return x<other.x;
			return y<other.y;
		}

		bool inBounds()const{
			return !(x<0||x>=(int64_t)src->width()||y<0||y>=(int64_t)src->height());
		}

		T* operator*() const{
			if(!inBounds())
				return nullptr;

			return &src->bd[y][x];
		}

		T* bufferAccess() const{
			if(!inBounds())
				return nullptr;

			return &src->buffer[y][x];
		}

		bool operator!=(const iter& other) const{
			return x!=other.x||y!=other.y;
		}

		bool operator==(const iter& other) const{
			return !(*this!=other);
		}
	};

	iter begin(){
		iter res;
		res.src=this;
		res.x=0;
		res.y=0;
		return res;
	}

	iter end(){
		iter res;
		res.src=this;
		res.x=0;
		res.y=height();
		return res;
	}

	iter at(uint64_t x,uint64_t y){
		iter res;
		res.src=this;
		res.x=x;
		res.y=y;
		return res;
	}

	void addRow(const std::vector<T>& data){
		if(bd.size()!=0&&bd[0].size()!=data.size())
			throw std::runtime_error("Trying to add row of different stride to board");
		bd.push_back(data);
	}

	void prepareBuffer(){
		buffer=bd;
	}

	void swapBuffer(){
		bd=buffer;
	}
/*
	friend ostream& operator<<(ostream& o,const board<T>&b){
		for(auto t:b.bd)
		{
			for(auto el:t)
				o<<(el?'#':'.');
			o<<endl;
		}
		return o;
	} */

	int64_t path(
			uint64_t initial_x,uint64_t initial_y,
			uint64_t end_x,uint64_t end_y,
			const std::function<int64_t (const iter& from,const iter& to)> value,
			int64_t initial_cost=0,	bool use8=false)
	{
		iter b,e;
		b.src=e.src=this;
		b.x=initial_x,b.y=initial_y;
		e.x=end_x,e.y=end_y;

		if(!b.inBounds()||!e.inBounds())
			throw runtime_error("Can't find shortest path, coordinates out of bound");

		std::map<iter,int64_t> costs;
		std::deque<iter> tovisit;
		tovisit.push_back(b);
		costs[b]=initial_cost;

		while(tovisit.size())
		{
			auto e=tovisit.front();
			tovisit.pop_front();
			vector<iter> neighbours;

			if(use8)
				neighbours=e.around8();
			else
				neighbours=e.around4();

			for(auto nb:neighbours)
			{
				if(!nb.inBounds())
					continue;

				auto move_cost=value(e,nb);
				if(!costs.count(nb)||costs[nb]>costs[e]+move_cost)
				{
					costs[nb]=costs[e]+move_cost;
					tovisit.push_back(nb);
				}
			}
		}

		if(!costs.count(e))
			return -1;
		return costs[e];
	}

	std::vector<row> bd;

private:
	std::vector<row> buffer;
};

template<class T>
ostream& operator<<(ostream&o, board<T>&b){
	for(auto v:b.bd){
		for(auto vv:v)
			cout<<vv;
		cout<<endl;
	}

	return o;
}

template<class T>
ostream& operator<<(ostream&o,vector<T>& r){
	bool first=true;
	for(auto& v:r)
	{
		if(first){
			first=false;
		}else
			o<<",";
		o<<v;
	}
	return o;
}

#endif

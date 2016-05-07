/*
 Student Name: Begun Unal
 Student Number: 2013400021
 Project Number: 2
 Operating System: Virtu1 Machine
 Compile Status: {Compiling/Not Compiling}
 Program Status: {Working/Not Working}
 Notes: Anything you want to say about your code that will be helpful in the grading process.
 */
#include <iostream>
#include <fstream>
#include <queue>
#include <iomanip>
using namespace std;

struct Event {
	double arrival, curTime;
	pair<int,int> where;
	double wait;
	Event()
	{

	}
	Event(double _arrival, pair<int,int> _where){
		arrival = _arrival;
		curTime = _arrival;
		where = _where;
		wait = 0;
	}
	bool operator<(Event other) const {
		return curTime > other.curTime;
	}

};
struct Machine {
	bool isempty;
	double finish, process;
	queue<Event>* inq;
	queue<Event>* outq;
	Machine();
	Machine(bool _isempty, double _process, queue<Event>* _inq,
			queue<Event>* _outq) {
		isempty = _isempty;
		process = _process;
		finish = 0;
		inq = _inq;
		outq = _outq;
	}
};
double start;
double maxWait = 0;
double totTurn = 0;
double totWait = 0;
queue<Event> q1;

queue<Event> q2;

priority_queue<Event> God;
vector<Machine> firstL;
vector<Machine> secondL;
int length1=0;
int length2=0;
int importData(char* infilename) {
	ifstream reader(infilename);
	double x;
	int dim1;
	int dim2;
	int package;
	reader >> dim1;
	//Machine* firstL[5000];
	for (int i = 0; i < dim1; i++) {
		reader >> x;
		firstL.push_back(Machine(true, x, &q1, &q2));
	}
	reader >> dim2;
	//Machine* secondL[5000];
	for (int i = 0; i < dim2; i++) {
		reader >> x;
		secondL.push_back(Machine(true, x, &q2, NULL));
	}
	reader >> package;
	//priority_queue<Event> God;

	for (int i = 0; i < package; i++) {
		reader >> x;
		Event event(x,make_pair(0,0));
		God.push(event);
	}
	return package;
}
int importData2(char* infilename) {
	ifstream reader(infilename);
	double x;
	int dim1;
	int dim2;
	int package;
	reader >> dim1;
	//Machine* firstL[5000];
	for (int i = 0; i < dim1/2; i++) {
		queue<Event> * cokq = new queue<Event>();
		for(int j=0; j<2;j++){
		reader >> x;
		firstL.push_back(Machine(true, x, &q1, cokq));
		}
	}
	reader >> dim2;
	//Machine* secondL[5000];
	for (int i = 0; i < dim2; i++) {
		reader >> x;
		secondL.push_back(Machine(true, x, firstL[2*i].outq, NULL));
	}
	reader >> package;
	//priority_queue<Event> God;

	for (int i = 0; i < package; i++) {
		reader >> x;
		Event event(x,make_pair(0,0));
		God.push(event);
	}
	return package;
}
void factory1(Event &e){
	if(e.where.first == 0){
		unsigned int i;
		for(i =0; i<firstL.size(); i++ ){
			if(firstL[i].isempty){
				e.where = make_pair(2,i);
				firstL[i].isempty = false;
				firstL[i].finish = e.curTime + firstL[i].process;
				e.curTime = firstL[i].finish;
				God.push(e);
				break;
			}
		}
		if(e.where.first == 0){
			e.where.first = 1;
			firstL[i-1].inq->push(e);

		}

	}else if(e.where.first == 1){
		unsigned int i;
		for(i =0; i<firstL.size(); i++ ){
			if(firstL[i].isempty){
				e.where = make_pair(2,i);
				firstL[i].isempty = false;
				firstL[i].finish = e.curTime + firstL[i].process;
				e.curTime = firstL[i].finish;
				God.push(e);
				break;
			}
		}
	}else if(e.where.first == 2){
		firstL[e.where.second].isempty = true;
		if(!firstL[e.where.second].inq->empty()){
			Event f = firstL[e.where.second].inq->front();
			f.wait += firstL[e.where.second].finish - f.curTime;
			f.curTime = firstL[e.where.second].finish;
			length1 = max(length1,(int)q1.size());
			firstL[e.where.second].inq->pop();
			factory1(f);
		}
		unsigned int i;
		for(i=0;i<secondL.size(); i++){
			if(firstL[e.where.second].outq == secondL[i].inq && secondL[i].isempty){
				e.where = make_pair(4,i);
				secondL[i].isempty = false;
				secondL[i].finish = e.curTime + secondL[i].process;
				e.curTime = secondL[i].finish;
				God.push(e);
				break;
			}
		}

		if(e.where.first == 2){
			e.where.first = 3;
			e.where.second /= 2;
			secondL[e.where.second].inq->push(e);
		}

	}else if(e.where.first == 3){
		unsigned i;
		for(i=0; i<secondL.size(); i++){
			if(secondL[e.where.second].inq == secondL[i].inq && secondL[i].isempty){
				e.where = make_pair(4,i);
				secondL[i].isempty = false;
				secondL[i].finish = e.curTime + secondL[i].process;
				e.curTime = secondL[i].finish;
				God.push(e);
				break;
			}
		}

	}else if(e.where.first == 4){
		secondL[e.where.second].isempty = true;
		if(!secondL[e.where.second].inq->empty()){
			Event g = secondL[e.where.second].inq->front();
			g.wait += secondL[e.where.second].finish-g.curTime;
			g.curTime = secondL[e.where.second].finish;
			length2 = max(length2,(int)secondL[e.where.second].inq->size());
			secondL[e.where.second].inq->pop();
			factory1(g);
		}
		totTurn += e.curTime - e.arrival;
		totWait +=e.wait;
		maxWait = max(maxWait,e.wait);

	}
}

int main(int argc, char *argv[]){
	if(argc!=3){
		printf("Usage: ./project1 infile outfile\n");
		return 0;
	}
	// int package = importData(argv[1]);
	int package = importData(argv[1]);
	Event e;
	int count = 0;
	while(!God.empty()){
		e = God.top();
		if(count == 0){
			start = e.arrival;
		}
		God.pop();
		factory1(e);
		count ++;
	}

	totWait = totWait/package;
	totTurn = totTurn/package;

	ofstream writer(argv[2]);

	writer<< fixed << setprecision(3) << totTurn<<endl;
	writer<< length1<<endl;
	writer<< length2<<endl;
	writer<< totWait<<endl;
	writer<< maxWait<<endl;
	writer<< e.curTime - start <<endl<<endl;

	totTurn = 0;
	length1 = 0;
	length2 = 0;
	totWait = 0;
	maxWait = 0;
	start = 0;
	firstL.clear();
	secondL.clear();

	int package2 = importData2(argv[1]);
	Event e2;
	int count2 = 0;
	while(!God.empty()){
		e2 = God.top();
		if(count2 == 0){
			start = e2.arrival;
		}
		God.pop();
		factory1(e2);
		count2 ++;
	}
	totWait = totWait/package2;
	totTurn = totTurn/package2;
	writer<< totTurn<<endl;
	writer<< length1<<endl;
	writer<< length2<<endl;
	writer<< totWait<<endl;
	writer<< maxWait<<endl;
	writer<< e2.curTime - start <<endl;


	return 0;
}

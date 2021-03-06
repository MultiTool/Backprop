#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

/* ********************************************************************** */
class Stack;// for layered 1-way networks
typedef Stack *StackPtr;
class Stack {
public:
  ClusterVec Layers;
  ClusterPtr InLayer, OutLayer;
  /* ********************************************************************** */
  Stack() {
  }
  /* ********************************************************************** */
  ~Stack() {
    for (int lcnt=0; lcnt<Layers.size(); lcnt++) {
      delete Layers.at(lcnt);
    }
  }
  /* ********************************************************************** */
  void Create_Simple() {
    ClusterPtr clprev, clnow;
    InLayer = clnow = new Cluster(3); Layers.push_back(clnow);
    clprev = clnow;
    int largo = 40;
    largo = 1;
    for (int lcnt=0; lcnt<largo; lcnt++) {
      //clnow = new Cluster(4); Layers.push_back(clnow);
      clnow = new Cluster(2+largo); Layers.push_back(clnow);
      clnow->Connect_Other_Cluster(clprev);
      clprev = clnow;
    }
    OutLayer = clnow = new Cluster(1); Layers.push_back(clnow);
    clnow->Connect_Other_Cluster(clprev);
  }
  /* ********************************************************************** */
  void Load_Inputs(double in0, double in1, double in2) {
    InLayer->NodeList.at(0)->FireVal = in0;
    InLayer->NodeList.at(1)->FireVal = in1;
    InLayer->NodeList.at(2)->FireVal = in2;
  }
  /* ********************************************************************** */
  void Fire_Gen() {
    int lcnt;
    ClusterPtr clnow;
    clnow = Layers.at(0);
    for (lcnt=1; lcnt<Layers.size(); lcnt++) {
      clnow->Push_Fire();// emit
      clnow = Layers.at(lcnt);
      clnow->Collect_And_Fire();// absorb
    }
  }
  /* ********************************************************************** */
  void Backprop(double goal) {
    int lcnt;
    ClusterPtr clnow;

    double FireVal = OutLayer->NodeList.at(0)->FireVal;
    OutLayer->NodeList.at(0)->Corrector = goal-FireVal;

    int LastLayer = Layers.size()-1;
    clnow = Layers.at(LastLayer);
    for (lcnt=LastLayer-1; lcnt>=0; lcnt--) {
      clnow->Push_Correctors_Backward();
      clnow = Layers.at(lcnt);
      clnow->Pull_Correctors();// absorb
    }

    for (lcnt=LastLayer; lcnt>=0; lcnt--) {
      clnow = Layers.at(lcnt);
      clnow->Apply_Correctors();
    }
  }
  /* ********************************************************************** */
  void Print_Me() {
    size_t cnt;
    ClusterPtr cluster;
    printf(" Stack this:%p, ", this);
    size_t siz = this->Layers.size();
    printf(" num Clusters:%li\n", siz);
    for (cnt=0; cnt<siz; cnt++) {
      cluster = this->Layers.at(cnt);
      cluster->Print_Me();
    }
  }
};

#endif // STACK_H_INCLUDED

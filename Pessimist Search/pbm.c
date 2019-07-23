#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>       /* time */
#include <stdbool.h>
#include <math.h>

#define TAM 10
#define heuristic 0 //Weight of heuristic chance

typedef struct {
  void* head;
  void * v;
  int l;
} list;


list* new_list() {
  list* def = malloc(sizeof(list));
  (*def).l = 0;
  (*def).head = NULL;
  (*def).v = NULL;
  return def;
}

bool empty(list* xs) {
  return (*xs).l == 0 ? true : false;
}

list* hd(list* xs) {
  list* tmp = (*xs).head;
  return tmp;
}

list* cons(list* xs, void*v) {
  list* new_list = malloc(sizeof(list));
  (*new_list).head = xs;
  (*new_list).v = v;
  (*new_list).l = xs->l + 1;
  return new_list;
}

void* car(list* xs) {
  return (*xs).v;
}

list* free_hd(list* xs) {
  list* tmp = (*xs).head;
  free(xs);
  return tmp;
}

int len(list* xs) {
  return (*xs).l;
}

void* fold(list* xs, void* f, void *a) {
  void *r = a;
  list* ls = xs;
  
  while(!empty(ls)) {
   r = ((void* (*)(void*, void*))f)(r, car(ls));
   ls = hd(ls);
  }

  return r;
}

void* fold_free(list* xs, void* f, void *a) {
  void *r = a;
  void* tmp;
  list* ls = xs;
  while(!empty(ls)) {
   tmp = car(ls);
   ls = free_hd(ls);
   r = ((void* (*)(void*, void*))f)(r, tmp);
  }
  return ls;
}


void* filter(list* xs, void* f) {
  void *ls = new_list();
  
  while(!empty(xs)) {
    if (((bool (*)(void*))f)(car(xs))) {
      ls = cons(ls, car(xs));
     }
  
    xs = hd(xs);
  }


  return ls;
}
void* Nothing (void* _, void* __) {return NULL;}

typedef struct {
  int x;
  int y;
} pos;

typedef enum {
  NO_REWARD = 0,
  WEAK_REWARD = 1,
  MEDIUM_REWARD = 2,
  STRONG_REWARD = 3
} reward;

typedef struct {
  pos position;
  int king;
  reward H;
} Agent;

typedef struct {
  list* agents;
  reward rew;
} Floor;

typedef Floor** Map;

typedef struct {
    int len;
    int agents_len;
    Map map;
    pos* positions;
} Enviroment;


Agent* create_agent(pos p, bool k) {
  Agent* m = (Agent*)malloc(sizeof(Agent));
  (*m).position = p;
  (*m).king = k;
  return m;
}

Floor* create_space(int n, int q) {
  Floor* f = malloc(sizeof(Floor)*n);
  for (int i=0; i <= n - 1; i++) {
    f[i].rew = 0;
    f[i].agents = new_list();
  }
  return f;
}


pos getRandFreePos(Map map, int l) {

  int x = rand() % l;
  int y = rand() % l;
  while (len(map[x][y].agents) > 0 || map[x][y].rew)  {
    x = rand() % l;
    y = rand() % l;
  }

  return (pos){x, y};
}

Enviroment put_agents(Map map_n, int t, int q) { 
  pos sort = getRandFreePos(map_n, t);
  map_n[sort.x][sort.y].agents = cons(map_n[sort.x][sort.y].agents, create_agent((pos){sort.x, sort.y}, 0));

  for (int _=0; _ <= q - 1; _++) {
    sort = getRandFreePos(map_n, t);
    map_n[sort.x][sort.y].agents = cons(map_n[sort.x][sort.y].agents, create_agent((pos){sort.x, sort.y}, 0));
  }

  return (Enviroment){t, q, map_n, malloc(sizeof(pos)*(q + 4))};
}

Enviroment create_map(int len) {
  int q = (rand() % (int)(len*0.5)) + 1;
  Floor** map = malloc(sizeof(Floor*)*len);
  for (int i=0; i<= len-1; i++) {
    map[i] = create_space(len, q);
  }

  return put_agents(map, len, q);
}

Enviroment putRewardPoint(Enviroment e) {
    pos pos_r[3];
    pos_r[0] = getRandFreePos(e.map, e.len);
    e.map[pos_r[0].x][pos_r[0].y].rew = 1;
    pos_r[1] = getRandFreePos(e.map, e.len);
    e.map[pos_r[1].x][pos_r[1].y].rew = 2;
    pos_r[2] = getRandFreePos(e.map, e.len);
    e.map[pos_r[2].x][pos_r[2].y].rew = 3;
    return e;
}


void print_map(Enviroment e) {
  char buffer[100];
  char min_buffer[25];
  bool k;
  char* kind_r;

  bool hasKing(list* agents) {

    void* choose_king(void* b, void* m) {
      *(bool*)b = (*(bool*)b) || (*(Agent*)m).king;
      return (void*)b;
    }

    bool* b = malloc(sizeof(bool*));
    (*b) = 0;

    bool _b = *(bool*)fold(agents, choose_king, b);

    free(b);
  
    return _b;
  }

  for (int i=0; i <= e.len -1; i++) {
    for (int i_=0; i_<=e.len - 1; i_++) {
     k = hasKing(e.map[i][i_].agents);
     kind_r = e.map[i][i_].rew == 1 ? "W" : (e.map[i][i_].rew == 2) ? "M" : "S";
     sprintf(min_buffer, "%d", len(e.map[i][i_].agents));
     sprintf(buffer, "%s%s", (len(e.map[i][i_].agents) == 0 ? ((k && e.map[i][i_].rew) ? "" : "-") : min_buffer), e.map[i][i_].rew ? kind_r : k ? "K" : "");
     printf("%s", buffer);
    }
    printf("\n");
  }

  return;
}



pos* getAllPosAgent(Enviroment e, pos* positions) {
    int n=0;

    for (int i=0; i <= e.len - 1; i++) {
        for (int _=0; _ <= e.len - 1; _++)  {
            if (len(e.map[i][_].agents)> 0) {
                   void* add_in_position(void* m, void* m_) {
                         positions[n++] = ((Agent*)m_)->position;
                         return NULL;
                   }

                  fold(e.map[i][_].agents, add_in_position, NULL);
           }

            if (e.map[i][_].rew) {
                 positions[n++] = (pos){i, _};
             }
        }
    }

    return positions;
}


pos searchPoint(Enviroment e, pos* positions, Agent m) {

    int calc_pos_dis(pos o, pos dis) {
        return abs(o.x - dis.x) + abs(o.y - dis.y);
    }

    pos pos_prox;
    bool b = false;
    bool v;

    for (int i=0; i <= e.agents_len + 3; i++) {
        v = ((m.position.x != positions[i].x) || (m.position.y != positions[i].y)) && (m.H <= e.map[positions[i].x][positions[i].y].rew);
        if (m.king && e.map[positions[i].x][positions[i].y].rew) {
          pos_prox = positions[i];
          b = true;
        }
        else if (!m.king && ((!b && v) || (v && calc_pos_dis(m.position, positions[i]) < calc_pos_dis(m.position, pos_prox)))) {
            pos_prox = positions[i];
            b = true;
        }
    }


    return b ? pos_prox : m.position;
}



Enviroment move_agents_to(Enviroment e, Agent* m, pos position) {

  bool filter_by_agent_pointer(void *m_) {
      return (m_ != m);
   }

  pos d__(int x, int y, int x_, int y_) {
    return (pos){x + (x > x_ ? -1 : (x == x_) ? 0 : 1), y + (y > y_ ? -1 : (y == y_) ? 0 : 1)};
  }
    
  pos npos = d__(m->position.x, m->position.y, position.x, position.y);

  if (m-> H >= e.map[m->position.x][m->position.y].rew && npos.x < e.len && npos.y < e.len && npos.x >= 0 && npos.y >= 0) {
    e.map[m->position.x][m->position.y].agents = filter(e.map[m->position.x][m->position.y].agents, filter_by_agent_pointer);

    (*m).position = (pos){npos.x, npos.y};
    (*m).H = (*m).H > e.map[npos.x][npos.y].rew ? (*m).H : e.map[npos.x][npos.y].rew;
    e.map[npos.x][npos.y].agents = cons(e.map[npos.x][npos.y].agents, m);
    

  }

}


Enviroment interaction (Enviroment e) {
    bool pertubation_enviroment() {
      pos* positions = getAllPosAgent(e, e.positions);
      bool meta_heuristic(list* agents) {
          Agent* m = car(agents);
  
          return m->H > e.map[m->position.x][m->position.y].rew ? true : (!(rand() % 10));
      }

      int sort[2];
      Agent* m;

      pos _ = positions[rand() % (e.agents_len + 3)];
      int q = 0;

      if (len(e.map[_.x][_.y].agents) > 0 && meta_heuristic(e.map[_.x][_.y].agents)) {
        m = car(e.map[_.x][_.y].agents);
        bool filter_by_agent_pointer(void *m_) {
          return (m_ != m);
         }

        e.map[_.x][_.y].agents = filter(e.map[_.x][_.y].agents, filter_by_agent_pointer);
        sort[0] = rand() % (e.len - 1);
        sort[1] = rand() % (e.len - 1);
        (*m).position = (pos){sort[0], sort[1]};
        e.map[sort[0]][sort[1]].agents = cons(e.map[sort[0]][sort[1]].agents, m);
      }

    }

    pos* positions = getAllPosAgent(e, e.positions);

    reward* exp_ = malloc(sizeof(reward));
    (*exp_) = NO_REWARD;
   
    for (int i=0; i <= e.agents_len + 3 ; i++) {
      void* interact_agent(void* m, void* m_) {
          e = move_agents_to(e, (Agent*)m_, searchPoint(e, positions, *(Agent*)m_));
       }


      void* getMaxExp(void* m, void* m_) {
        reward n = ((Agent*)m_)->H;
        reward exp_ = (*(reward*)m);
        (*(reward*)m_) = exp_ > n ? exp_ : n; 
        return m_;
       }

      void* shareExp(void* m, void* m_) {
        reward exp_ = (*(reward*)m); 
        ((Agent*)m_)->H = exp_;
        return m_;
       }


         if (!e.map[positions [i].x][positions[i].y].rew) {
           fold(e.map[positions [i].x][positions[i].y].agents, interact_agent, NULL);
           fold(e.map[positions [i].x][positions[i].y].agents, getMaxExp, exp_);
           fold(e.map[positions [i].x][positions[i].y].agents, shareExp, exp_);
         }


      }

      free(exp_);
   pertubation_enviroment();


  return e;
}



int main() {
  
  srand(time(NULL));

  Enviroment k = putRewardPoint(create_map(TAM));

  int i = 0;
  while (i <= 100) {
    print_map(k);
    k = interaction(k);

    printf("\n \n");
    i++;
  }


  return 0;
}
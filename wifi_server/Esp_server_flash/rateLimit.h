class Ratelimit{
  private:
    uint8_t _counter=0;
    Struct _Client{
      String ip;
      int count;
      bool canPass;
      long firstVisitTime;
      long imprisonedTime;
    }

    _Client *C[10];
    void _makePerson(){//this method helps init the decalred array
      for(uin8_t x=0; x<10; x++){
        c[x] = new _Client;
        c[x]->canPass=true;
      }
    }
  public:
    void init(){
      _makePerson();
    }
    bool cannPass(const String clientIp){
      uint8_t cIndex;
      _counter+=1;
      for(uint8_t x=0; x<10; x++){
        if((c[x]->ip)==clientIp || (c[x]->ip).length<1){//helps check if the ip address already exist in the list of exisiting visitors
          cIndex=x;
          c[x]->ip=clientIp;
          c[x]->count=_counter;
          if(_counter==1) c[x]->firstVisitTime=int(millis()/1000);

          if((_counter>10 && ((millis()/1000)-c[x]->firstVisitTime)<60)){//line check if client made above 1o request within 1 min
              c[x]->canPass=false;
              c[x]->imprisonedTime=millis()/1000;
          }
          else{
            c[x]->canPass=true;
          }
          break;
        }
      }
      return c[cIndex]->canPass;
    }

    void freePrisoner(){//this method helps free it after it is done serving 60sec jail time
       for(uint8_t x=0; x<10; x++){
        if(c[x]->canPass==false){
          if((millis()/1000)-c[x]->imprisonedTime)>=60) {
            c[x]->canPass=true;
            c[x]->count=0;
        }
       }
       if(c[x]->count>10 && c[x]->canPass==true){
         c[x]->count=0;
       }
    }}
};
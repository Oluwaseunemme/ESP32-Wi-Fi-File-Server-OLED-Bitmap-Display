class Ratelimit{
  private:
    const uint8_t _counterLimit=5;
    struct _Client{
      String ip;
      int count;
      bool canPass;
      long firstVisitTime;
      long imprisonedTime;
    };

    _Client *c[10];
    void _makePerson(){//this method helps init the decalred array
      for(uint8_t x=0; x<10; x++){
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

      for(uint8_t x=0; x<10; x++){
        if((c[x]->ip)==clientIp || (c[x]->ip).length()<2){//helps check if the ip address already exist in the list of exisiting visitors
          (c[x]->ip)==clientIp? Serial.println("Old client here ....."):Serial.println("new client received and stored....");
          if(!c[x]->canPass) return false;
          cIndex=x;
          c[x]->ip=clientIp;
          c[x]->count+=1;
          if(c[x]->count==1) c[x]->firstVisitTime=int(millis()/1000);

          if((c[x]->count>=_counterLimit && ((millis()/1000)-c[x]->firstVisitTime)<60)){//line check if client made above 1o request within 1 min
              c[x]->canPass=false;
              c[x]->imprisonedTime=millis()/1000;
              Serial.println("Police caught a thieve...");
              Serial.printf("%s user is now locked\n", c[x]->ip);
          }
          if(c[x]->count>=_counterLimit && c[x]->canPass){//helps reset limit for individuals with 10+ count without breaking limit
            c[x]->firstVisitTime=int(millis()/1000);
          }
          break;
        }
      }
      return c[cIndex]->canPass;
    }

    void freePrisoner(){//this method helps free it after it is done serving 60sec jail time
       for(uint8_t x=0; x<10; x++){
        if(c[x]->canPass==false){
          if((millis()/1000)-(c[x]->imprisonedTime)>=60) {
            c[x]->canPass=true;
            c[x]->count=0;
            Serial.printf("%s user is now freed from prison\n", c[x]->ip);
        }
       }
       if(c[x]->count>_counterLimit && c[x]->canPass==true){
         c[x]->count=0;
       }
       //Serial.println("ALL motherfuckers freed....");
    }}
};
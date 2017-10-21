 String input="t234AB00088,t34ERDS";

 
void setup() {
  // put your setup code here, to run once:  
 
    Serial.begin(115200); 
    input.reserve(200);
}
 
void loop() {
  // put your main code here, to run repeatedly:   
 int templen=input.length();
 String tempstring="";
 for(int i=0;i<templen;i++) {
  int n=0;
  if(input[i]==','){
    for(int j=0;j<n;j++){
      char tempchar=tempstring.charAt(j);
      Serial.write(tempchar);
      //Serial.print(tempchar);
      }
    n=0;
    continue;
    }
    tempstring[n++]=input[i];
  }   
  
}
  

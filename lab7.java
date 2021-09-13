

/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Codigo: Leitores e escritores usando monitores em Java */
/* -------------------------------------------------------------------*/

// Monitor que implementa a logica do padrao leitores/escritores

class numero{
    private int num;
 
    public numero(){
        this.num = 0;
    }
 
    public synchronized String ÉPar(int num){
        if(num % 2 == 0) {
            return " é par";
        } else return" é impar";
    }
 
    public synchronized String ÉPrimo(int num){
        for (int j = 2; j < num; j++) {
            if (num % j == 0){
                return " não é primo";
            }
        }
        return " é primo";
    }
 
    public int get() { 
        return this.num; 
    }
 
    public synchronized void set(int n) { 
        this.num = n; 
    }
  }
  
 
 class LE {
  private int leit, escr;
  // Construtor
  LE() { 
     this.leit = 0; //leitores lendo (0 ou mais)
     this.escr = 0; //escritor escrevendo (0 ou 1)
  }
 
  // Entrada para leitores
  public synchronized void EntraLeitor (int id) {
    try { 
      //while (this.escr > 0) {
      if (this.escr > 0) {
         System.out.println ("le.leitorBloqueado("+id+")");
         wait();  //bloqueia pela condicao logica da aplicacao 
      }
      this.leit++;  //registra que ha mais um leitor lendo
      System.out.println ("le.leitorLendo("+id+")");
    } catch (InterruptedException e) { }
  }
  
  // Saida para leitores
  public synchronized void SaiLeitor (int id) {
     this.leit--; //registra que um leitor saiu
     if (this.leit == 0) 
           this.notify(); //libera escritor (caso exista escritor bloqueado)
     System.out.println ("le.leitorSaindo("+id+")");
  }
  
  // Entrada para escritores
  public synchronized void EntraEscritor (int id) {
    try { 
      //while ((this.leit > 0) || (this.escr > 0)) {
      if ((this.leit > 0) || (this.escr > 0)) {
         System.out.println ("le.escritorBloqueado("+id+")");
         wait();  //bloqueia pela condicao logica da aplicacao 
      }
      this.escr++; //registra que ha um escritor escrevendo
      System.out.println ("le.escritorEscrevendo("+id+")");
    } catch (InterruptedException e) { }
  }
  
  // Saida para escritores
  public synchronized void SaiEscritor (int id) {
     this.escr--; //registra que o escritor saiu
     notifyAll(); //libera leitores e escritores (caso existam leitores ou escritores bloqueados)
     System.out.println ("le.escritorSaindo("+id+")");
  }
 }
 
 
 
 //Aplicacao de exemplo--------------------------------------------------------
 // Leitor
 class Leitor extends Thread {
  int id; //identificador da thread
  int delay; //atraso bobo
  LE monitor;//objeto monitor para coordenar a lógica de execução das threads
  numero num;
 
  // Construtor
  Leitor (int id, int delayTime, LE m, numero num) {
    this.id = id;
    this.delay = delayTime;
    this.monitor = m;
    this.num = num;
  }
 
  // Método executado pela thread
  public void run () {
    double j=777777777.7, i;
    try {
      for (;;) {
        this.monitor.EntraLeitor(this.id);
        System.out.println ("O número " + this.num.get() + this.num.ÉPrimo(this.num.get()));
        for (i=0; i<100000000; i++) {j=j/2;} //...loop bobo para simbolizar o tempo de leitura
        this.monitor.SaiLeitor(this.id);
        sleep(this.delay); 
      }
    } catch (InterruptedException e) { return; }
  }
 }
 
 //--------------------------------------------------------
 // Escritor
 class Escritor extends Thread {
  int id; //identificador da thread
  int delay; //atraso bobo...
  LE monitor; //objeto monitor para coordenar a lógica de execução das threads
  numero num;
 
  // Construtor
  Escritor (int id, int delayTime, LE m, numero num) {
    this.id = id;
    this.delay = delayTime;
    this.monitor = m;
    this.num = num;
  }
 
  // Método executado pela thread
  public void run () {
    double j=777777777.7, i;
    try {
      for (;;) {
        this.monitor.EntraEscritor(this.id); 
        this.num.set(this.id);
        for (i=0; i<100000000; i++) {j=j/2;} //...loop bobo para simbolizar o tempo de escrita
        this.monitor.SaiEscritor(this.id); 
        sleep(this.delay); //atraso bobo...
      }
    } catch (InterruptedException e) { return; }
  }
 }
 
 //--------------------------------------------------------
 // Escritor-Leitor
 class EscritorLeitor extends Thread {
    int id; //identificador da thread
    int delay; //atraso bobo...
    LE monitor; //objeto monitor para coordenar a lógica de execução das threads
    numero num;
  
    // Construtor
    EscritorLeitor (int id, int delayTime, LE m, numero num) {
      this.id = id;
      this.delay = delayTime;
      this.monitor = m;
      this.num = num;
    }
  
    // Método executado pela thread
    public void run () {
      double j=777777777.7, i;
      try {
        for (;;) {
          this.monitor.EntraLeitor(this.id);
          System.out.println ("O número " + this.num.get() + this.num.ÉPar(this.num.get()));
          for (i=0; i<100000000; i++) {j=j/2;} //...loop bobo para simbolizar o tempo de leitura
          this.monitor.SaiLeitor(this.id);
          sleep(this.delay); //atraso bobo...
 
          this.monitor.EntraEscritor(this.id); 
          this.num.set(this.num.get() * 2);
          for (i=0; i<100000000; i++) {j=j/2;} //...loop bobo para simbolizar o tempo de escrita
          this.monitor.SaiEscritor(this.id); 
          sleep(this.delay); 
        }
      } catch (InterruptedException e) { return; }
    }
  }
 
 //--------------------------------------------------------
 // Classe principal
 class LeitorEscritor {
  static final int L = 4;
  static final int E = 2;
  static final int LE = 4;
 
  public static void main (String[] args) {
    int i;
    LE monitor = new LE();            // Monitor (objeto compartilhado entre leitores e escritores)
    Leitor[] l = new Leitor[L];       // Threads leitores
    Escritor[] e = new Escritor[E];  // Threads escritores
    EscritorLeitor[] le = new EscritorLeitor[LE];  // Threads escritores-leitores
 
    //cria uma instancia do recurso compartilhado entre as threads
    numero num = new numero();
 
    //inicia o log de saida
    System.out.println ("import verificaLE");
    System.out.println ("le = verificaLE.LE()");
    
    for (i=0; i<L; i++) {
       l[i] = new Leitor(i+1, (i+1)*500, monitor, num);
       l[i].start(); 
    }
    for (i=0; i<E; i++) {
       e[i] = new Escritor(i+1, (i+1)*500, monitor, num);
       e[i].start(); 
    }
 
    for (i=0; i<LE; i++) {
        le[i] = new EscritorLeitor(i+1, (i+1)*500, monitor, num);
        le[i].start(); 
     }
  }
 }
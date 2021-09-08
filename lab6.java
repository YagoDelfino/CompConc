
//classe da estrutura de dados (recurso) compartilhado entre as threads
class S {
  //recurso compartilhado
  private int r;
  //construtor
  public S() { 
     this.r = 0; 
  }

  //operacao de escrita sobre o recurso compartilhado
  public synchronized void inc() { 
     this.r++; 
  }
  //operacao de leitura sobre o recurso compartilhado
  public int get() { 
     return this.r; 
  }

}

//classe que estende Thread e implementa a tarefa de cada thread do programa 
class T extends Thread {
   //identificador da thread
   private int id;
   private int vector[];
   private int nthreads;
   //objeto compartilhado com outras threads
   S s;
  
   //construtor
   public T(int tid, S s, int v[], int nthreads) { 
      this.id = tid; 
      this.s = s;
      this.nthreads = nthreads;
      this.vector = v;
   }

   //metodo main da thread
   public void run() {
      int inicio = this.id * (this.vector.length/this.nthreads);
      int fim;
    
      if(this.id == this.nthreads - 1) fim = this.vector.length;
      else fim = inicio + (this.vector.length/this.nthreads);

      System.out.println("Thread " + this.id + " iniciou!");
      System.out.println("De " + inicio + " até " + fim);
      for (int i= inicio; i<fim; i++) {
         if((this.vector[i] % 2) == 0) this.s.inc();  
      }
      System.out.println(this.s.get());
      System.out.println("Thread " + this.id + " terminou!"); 
   }
}

//classe da aplicacao
class Pares {
   static final int N = 4;

   public static void main (String[] args) {
      //reserva espaço para um vetor de threads
      Thread[] threads = new Thread[N];
      int v[] = new int[100];

      //cria uma instancia do recurso compartilhado entre as threads
      S s = new S();

      //inicializa o vetor
      for(int i = 0; i <v.length; i++){
         v[i] = i%10;
      }

      //cria as threads da aplicacao
      for (int i=0; i<threads.length; i++) {
         threads[i] = new T(i, s, v, N);
      }

      //inicia as threads
      for (int i=0; i<threads.length; i++) {
         threads[i].start();
      }


      //espera pelo termino de todas as threads
      for (int i=0; i<threads.length; i++) {
         try { threads[i].join(); } catch (InterruptedException e) { return; }
      }

      System.out.println("Valor de números pares = " + s.get()); 
   }
}


 #include <stdio.h>
 #include <stdlib.h>

 /* Inclusao das funcoes que contem o metodo Rubro-negra */
 #include "arvB.c"




 int main()
 {

    tipoArvB *arv;
    arv = NULL;


    arv = insereArvB (arv,1);
    arv = insereArvB (arv,20);
    arv = insereArvB (arv,30);
    arv = insereArvB (arv,15);
    //arv = insereArvB (arv,14);
    
    printf("Arvore B: \n");
    imprime_arvore(arv);
    printf("\n\n\n");

}
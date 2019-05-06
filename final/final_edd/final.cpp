
/*
* Árbol binario autoorganizado.
*/
#include <stdlib.h>
#include <stdio.h>
#include "final.h"
#include <iostream>
#include <stdio.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
using namespace std;
#include <fstream>
//prototipos de funciones locales
arbol sucesor(arbol t) ;
static arbol join(arbol, arbol);
arbol descartar(tipoclave valor, arbol t);
static arbol lrot(arbol);
static arbol rrot(arbol);
static arbol CreaNodo(tipoclave);
static void LiberaNodo(arbol);
static void Error(int,tipoclave);
void ImprimeNodo(arbol t, int h); //test
void MuestraArbol(arbol t, int h);
arbol insertarrecursivo(tipoclave valor, arbol T);
arbol CreaArbol(arbol t, tipoclave a[]);
//Variables Globales y Definiciones.
static arbol NodoInsercion=NULL; /* Variable temporal, usada en insert, y por lo tanto en splay*/
static int flag; /* variable de estado */
/*
* Bottom up
*/
#define Root 0
#define Zag 1
#define Zig 2
#define NotFind 0
#define Find 1
#define Zig_Zig 2
#define Zig_Zag 3
#define Zag_Zag 4
#define Zag_Zig 5

arbol splay(tipoclave valor, arbol t, int fw)
{
if (t == NULL && NodoInsercion == NULL) {
flag=NotFind; /* árbol vacio o no lo encontró en búsqueda*/
return NULL;
}
else if (t == NULL && NodoInsercion != NULL) { /* encuentra posición para insertar */
t=NodoInsercion; /* Lo inserta */
NodoInsercion=NULL; /* Limpia variable global */
flag=Find; //comienza el ascenso y la operación splay.
return t;
}
else if (t->clave == valor) { /* Lo encuentra antes de llegar a una hoja */
flag=Find; //comienza operación splay. No marca global NodoInsercion (3).
return t; //retorna puntero al encontrado
}

else if (t->clave < valor) {
t->right=splay(valor,t->right, Zag); //desciende por la derecha
if (flag) { /* rotaciones sólo si estaba en el árbol */
if (flag==Zag_Zag) {
t=lrot(t);
t=lrot(t); //efectúa doble rotación LL
flag=Find; //resetea al ascender dos niveles.
}
else if (flag==Zag_Zig) {
t=lrot(t); //rota el abuelo a la izquierda. (2)
flag=Find; //resetea después de la doble rotación.
}
else if (fw==Zag)
flag=Zag_Zag; //se juntan dos seguidas ascendiendo por la derecha
else if (fw==Zig) { //está procesando Zig, y la anterior era Zag.
t=lrot(t); //rota el padre a la izquierda (1)
flag=Zig_Zag;
}
else /* (fw==Root) */
t=lrot(t); //efectúa Zag, un nivel bajo la raíz
}
}
else { /* (t->clave < valor) */
t->left=splay(valor,t->left,Zig); //desciende por la izquierda
if (flag) { /* rotaciones sólo si estaba en el árbol */
if (flag==Zig_Zig){
t=rrot(t);
t=rrot(t); //efectúa doble rotación RR
flag=Find; //resetea al ascender dos niveles.
}
else if (flag==Zig_Zag){
t=rrot(t); //rota el abuelo a la derecha (1)
flag=Find; //resetea al ascender dos niveles.
}
else if (fw==Zig)
flag=Zig_Zig; //se juntan dos seguidas ascendiendo por la izquierda
else if (fw==Zag) { //está procesando Zag, y la anterior era Zig.
t=rrot(t); //rota el padre a la derecha (2)
flag=Zag_Zig;
}
else /* (fw==Root) */
t=rrot(t); //efectúa Zig, un nivel bajo la raíz
}
}
return t;
}

/*
* Top Down
*/
#define rlink(t) (*r=(t), r=&((t)->left), (t)=(t)->left)
#define llink(t) (*l=(t), l=&((t)->right), (t)=(t)->right)

arbol splayTD(tipoclave valor, arbol t)
{
arbol L=NULL, R=NULL; /* Subárboles */
arbol *l=&L, *r=&R; /* punteros para insertar en L y R*/
arbol p;
while (t != NULL && t->clave != valor) {
if( valor < t->clave) {
p = t->left; /*Desciende por la izquierda*/
if (p != NULL && valor < p->clave) { /* Zig_Zig */
 printf("Zig-Zig en %d\n",t->clave);
t=rrot(t);
rlink(t);
}
else if (p != NULL && valor > p->clave) { /* Zig_Zag */
 printf("Zig-Zag en %d\n",t->clave);
rlink(t);
llink(t);
}
else if(p != NULL && valor == p->clave) /* Zig */
{
printf("Lo encontró. Zig en %d\n",t->clave);
rlink(t);
}
else if(p==NULL && NodoInsercion !=NULL)
 {
 printf("Zig para insertar en %d\n",t->clave);
 rlink(t); //no está y debe insertarlo.
 }
else if ((p==NULL) && NodoInsercion==NULL)
 {
 printf("Sube %d. Splay con el padre del no encontrado\n",t->clave);
 break; //no está el buscado. sube el padre del no encontrado a la raíz
} }
else { /* (valor > t->clave) */
p = t->right; /*Desciende por la derecha*/
if (p != NULL && valor > p->clave) { /* Zag_Zag */
 printf("Zag-Zag en %d\n",t->clave);
t=lrot(t);
llink(t);
}
else if (p != NULL && valor < p->clave) { /* Zag_Zig */
 printf("Zag-Zig en %d\n",t->clave);
llink(t);
rlink(t);
}
else if(p!=NULL && valor == p->clave)/* Zag */
{
 printf("Lo encontró. Zag en %d\n",t->clave);
 llink(t);
}
else if(p==NULL && NodoInsercion !=NULL)
 {
 printf("Zag para insertar en %d\n",t->clave);
 llink(t); //no está y debe insertarlo.
 }
 else if ((p==NULL) && NodoInsercion==NULL)
 {
 printf("Sube %d .Splay con el padre del no encontrado\n",t->clave);
 break; //no está el buscado. sube el padre del no encontrado a la raíz
 }
}
}
if (t==NULL && NodoInsercion == NULL) { /* si busca y árbol vacío */
return t;
}
if (t == NULL && NodoInsercion != NULL) { /* */
t=NodoInsercion; /* inserta y lo deja en la raíz */
NodoInsercion=NULL; /* reinicia global */
}
if(L!=NULL) {*l = t->left; t->left =L; } /*join final*/
if(R!=NULL) {*r = t->right; t->right=R;}
return t;
}

/*
* insertar(valor, t): inserta nodo con clave igual a valor en arbol t
*/
arbol insertar(tipoclave valor, arbol t)
{
arbol p;
NodoInsercion = CreaNodo(valor); /* Crea el nodo y pega en la global */
 // p=splayBU(valor, t, Root); /* Si no lo encuentra, lo inserta y lo coloca en la raíz */
 p=splayTD(valor, t); /* Si no lo encuentra, lo inserta y lo coloca en la raíz */
if (NodoInsercion != NULL) { /* Si ya estaba, libera el nodo */

free(NodoInsercion);
NodoInsercion=NULL;
Error(1,valor); // Avisa error de inserción.
}
return p;
}
arbol buscar(tipoclave valor, arbol t)
{
arbol p;
NodoInsercion = NULL; /* */
//p=splayBU(valor, t, Root); /* si lo encuentra, lo coloca en la raíz. */
p=splayTD(valor, t); /* si lo encuentra, lo coloca en la raíz. */
if(p==NULL) Error(2,valor); // Busca en árbol vacío.
return p;
}
arbol sucesor(arbol t) /* Algoritmo iterativo */
/*menor descendiente de subárbol derecho */
{ arbol p;
 if(t!=NULL) p = t->right; else return(NULL);
 if(p==NULL) return(NULL);
 if (p->left == NULL) /* No hay hijo izq. */
 return (p); /* Retorna el menor */
 while ( p->left != NULL) { /* Mientras no tenga hijo izq descender por la izq */
 t = p;
 p = p->left;
 }
 /*Al terminar el while p apunta al menor descendiente */
 return (p); /* Retorna el menor */
}
arbol borrar(tipoclave valor, arbol t)
{
arbol p,q,r;
NodoInsercion = NULL; /* */
p=buscar(valor, t); /* si lo encuentra, lo coloca en la raíz. */
//MuestraArbol(p, 1);
if (p==NULL) return(NULL);
r=sucesor(p);
if(r!=NULL)
 {q=splayTD(r->clave, p->right);
 t=join(p->left,q);
 }
else t=p->left;
 LiberaNodo(p);
return(t);
}
/*
* descartar(valor, t): borra nodo con clave == valor en árbol t
* No se implementa mediante splay. Lo que se borra no se volverá a emplear.
*/
arbol descartar(tipoclave valor, arbol t)
{
arbol *p = &t;
arbol temp;
while (*p != NULL && (*p)->clave != valor) {//descenso iterativo
if((*p)->clave < valor)
p = &((*p)->right);
else
p = &((*p)->left);
}
if (*p != NULL) { /* (*p)->clave == valor. Encontró el nodo para descartar */
temp = *p;
/* Uno o dos hijos? */
if ((*p)->left == NULL) *p = (*p)->right;
else if ((*p)->right == NULL)*p = (*p)->left;
else /* si tiene dos hijos */
 *p =join((*p)->left,(*p)->right);
LiberaNodo(temp);
}
else /* No lo encontró */
Error(0,valor);
return(t);
}
void graficarGraph(){
FILE *f;
char direccion[] = "C:/release/proyectos/ffff.txt";
f = fopen(direccion,"w");
if(f==NULL){
//    cout << "no se creó el archivo" << endl;
}else{


    ofstream archivo;
    archivo.open(direccion);

    //--------------------Aqui inicia la escritura del archivo-------------------

    archivo << "digraph G{ " << endl;
    archivo << "node[shape = box];" << endl;
    archivo << t->clave<<endl;
    archivo<<"->"<<endl;
    archivo << "}" << endl;
     archivo<<"label=\"SPL\""<<endl;
    //---------------------Aqui termina el archivo----------------

    archivo.close();
       system("dot -Tpng /home/marcos/ffff.dot -o /home/marcos/Documentos/ffff.png");
        system("xdg-open /home/marcos/ffff.dot.png");
}

}


//join (l, r): Retorna el árbol formado por la combinación del árbol "l", y del árbol "r".
//Se asume que cualquier item en "l" tiene valores menores que cualquier item en "r".
static arbol join(arbol l, arbol r)
{
arbol t;
arbol *p = &t;
while (l != NULL && r != NULL) {
*p = l;
l = l->right;
(*p)->right = r;
p = &(r->left);
r = r->left;
}
if (l == NULL) *p = r;

else /* (r == NULL) */
*p = l;
return t;
}
static arbol lrot(arbol t)
{ arbol temp = t->right;
 t->right = temp->left;
 temp->left = t;
 return ( temp);
}
static arbol rrot(arbol t)
{ arbol temp = t->left;
 t->left = temp->right;
 temp->right = t;
 return (temp);
}
static void Error(int error, tipoclave valor)
{
if(error==1) printf("Error: Intenta insertar clave=%d existente!\n",valor);
else if(error==0) printf("Error: Intenta descartar clave=%d inexistente!\n", valor);
else if(error==2) printf("Error: Busca clave=%d en árbol vacío!\n", valor);
}
static nodo* CreaNodo(tipoclave valor)
{
arbol p;
p=(arbol)calloc(1, sizeof(nodo));
//p->nombre=(char*) NULL;
p->clave = valor;
p->left = NULL;
p->right = NULL;
return p;
}
static void LiberaNodo(arbol p)
{
//if (p->nombre != (char *)NULL) free(p->nombre);//libera string
free(p);
}
/*
int AlturaArbol(arbol t)
{
if (t == NULL) return 0;
else return 1//+max(AlturaArbol(t->left),AlturaArbol(t->right));
}*/
int ContarNodos(arbol t)
{
if (t == NULL) return 0;
else return 1+ContarNodos(t->left)+ContarNodos(t->right);
}
arbol BorraArbol(arbol t)
{
if (t != NULL) {
t->left=BorraArbol(t->left);
t->right=BorraArbol(t->right);
LiberaNodo(t);
}
return NULL;
}
/* end of splay.c */

void ImprimeNodo(arbol t, int h)
{ int i;
 for(i=0; i<h; i++) putchar('\t');
 if(t==NULL) { putchar('*') ; putchar('\n') ;}
 else printf("%d\n",t->clave);
}

void MuestraArbol(arbol t, int h)
{
 if(t==NULL) ImprimeNodo(t, h);
 else {

 MuestraArbol(t->right, h+1); ImprimeNodo(t, h); MuestraArbol(t->left, h+1);
 }

}

void graficarArray(arbol t, int h)
{ int i;
FILE *f;
char direccion[] = "C:/release/proyectos/grafica1.txt";
f = fopen(direccion,"w");
if(f==NULL){
    cout << "no se creó el archivo" << endl;
}else{


    ofstream archivo;
    archivo.open(direccion);

    //--------------------Aqui inicia la escritura del archivo-------------------

    archivo << "digraph G{ " << endl;
    archivo << "node[shape = box];" << endl;
    archivo << t->clave<<endl;
    archivo << "}" << endl;
     archivo<<"label=\"ARREGLO\""<<endl;

    //---------------------Aqui termina el archivo----------------

    archivo.close();
    system("C:/release/bin/dot.exe -Tpng C:/release/proyectos/grafica1.txt -o C:/release/proyectos/grafica1.png");
    system("C:/release/proyectos/grafica1.png");
}



 for(i=0; i<h; i++) putchar('\t');
 if(t==NULL) { putchar('*') ; putchar('\n') ;}
 else printf("%d\n",t->clave);
}

void MuestraArbol(arbol t, int h)
{
 if(t==NULL) ImprimeNodo(t, h);
 else {

 MuestraArbol(t->right, h+1); ImprimeNodo(t, h); MuestraArbol(t->left, h+1);
 }

}


arbol insertarrecursivo(tipoclave valor, arbol T) /* recursivo */
{
 if (T == NULL)
 { T = (arbol) malloc(sizeof(nodo));
 if (T == NULL) printf("Rebalse del heap!\n");
 else {T->clave = valor; T->left = T->right = NULL;}
 }
else
 if (valor < T->clave) T->left = insertarrecursivo(valor,T->left);
 else if (valor > T->clave) T->right = insertarrecursivo(valor,T->right);
 else Error(1,valor);
 return(T);
}
#define maxnodos 56
arbol CreaArbol(arbol t, tipoclave a[])
{ int i;
 for(i=0;i<maxnodos;i++)
 if(insertarrecursivo(a[i],t)==NULL) printf("error en inserción\n");
 return t;
}

/*Probar con:
*
* 4 4 4 4 4 4
* / \ / \ / \ / \ / \
* 2 6 2 6 2 6 2 6 2 6
* / \ / \
* 1 3 5 7
*/

//Variables para mantener el árbol.
arbol root=NULL;
tipoclave arr[maxnodos]={10,96,30,2,63,85,47,20,3,4,19}; //orden de ingreso de claves
int main(void)
{
root=insertarrecursivo(5, root); //crea raíz
CreaArbol(root,arr); //agrega nodos
MuestraArbol(root, 1);
graficarGraph();
graficarArray();
//comenzar test
//…………..
 //root=delarbol(root); //despedida limpia
 return 0;
}




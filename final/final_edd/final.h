typedef int tipoclave;
typedef struct moldenode {
tipoclave clave;/* Clave */
struct moldenode *left, *right;
} nodo, *arbol;
/* Definiciones de macros */
//#define max(A,B) ((A)>(B)?(A):(B))
//#define search(valor,t) ((t)=splayBU((valor),(t),0), ((t)!=NULL&&(t)->clave==(valor))?(t):NULL)
//funciones definidas en splay.c Pueden invocarse si se incluye splay.h
extern arbol splayBU(tipoclave, arbol, int);
extern arbol splayTD(tipoclave, arbol);
extern arbol insertar(tipoclave, arbol);
extern arbol borrar(tipoclave, arbol);
extern int AlturaArbol(arbol);
extern int ContarNodos(arbol);
extern arbol BorraArbol(arbol);
void graficarGraph();
void graficarArray();
/* end of splay.h */

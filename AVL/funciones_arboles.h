///STRUCTS Y DECLARACIONES:
///========================

struct _nodo{
    char *dato;
    struct _nodo *izquierda;
    struct _nodo *derecha;
    int height;
};

typedef struct _nodo Node;

#define calcAltura(x) ( 1 + maximo(height(x->izquierda), height(x->derecha)) )


/// FUNCIONES:
///===========

//FUNCIONES DE RECORRIDO
void inorden(Node *arbol){
    if (arbol != NULL){
        inorden(arbol->izquierda);
        printf("\n%s ", arbol->dato);
        inorden(arbol->derecha);
    }
}

void preorden(Node *arbol){
    if (arbol != NULL){
        printf("\n%s ", arbol->dato);
        preorden(arbol->izquierda);
        preorden(arbol->derecha);
    }
}

void postorden(Node *arbol){
    if (arbol != NULL){
        postorden(arbol->izquierda);
        postorden(arbol->derecha);
        printf("\n%s ", arbol->dato);
    }
}

Node* verSiEsta(Node *arbol, char* frase){
    /** ESTA FUNCION BUSCA SI UNA FRASE PERTENECE (DEVUELVE EL NODO)
        O NO PERTENECE (DEVUELVE NULL) AL ARBOL
    */
    if (!arbol){                                            /// NO SE ENCONTRO
        return NULL;
    }else if (strcmp(arbol->dato, frase) == 0){             /// ES ESA PALABRA
        return arbol;
    } else if (strcmp(arbol->dato, frase) < 0){             /// SEGUIR BUSCANDO POR LA DERECHA
        return verSiEsta(arbol->derecha, frase);
    } else {                                                /// SEGUIR BUSCANDO POR LA IZQUIERDA
        return verSiEsta(arbol->izquierda, frase);
    }
}

int height(Node *N){
    /** ESTA FUNCION CALCULA LA ALTURA DE UN NODO
        SE UTILIZA PARA BALANCEAR EL ARBOL
    */
    if (!N)
        return 0;
    return N->height;
}

int maximo(int a, int b){
    /// FUNCION UTILITARIA PARA AHORRAR CODIGO
    return (a > b)? a : b;
}

Node *newNode(char *dato){
    /** SE UTILIZA PARA CREAR UN NUEVO NODO
        CON HIJOS NULL
        Y EL DATO SE PASA COMO PARAMETRO
    */
    Node* nodo = (Node*) malloc(sizeof(Node));
    nodo->dato = malloc(strlen(dato) + 1);
    strcpy(nodo->dato, dato);
    nodo-> izquierda = NULL;
    nodo-> derecha = NULL;
    nodo-> height = 1; /// DESPUES SE MODIFICA
    return nodo;
}

// LAS ROTACIONES SE EXPLICAN EN OTRO ARCHIVO
Node *rotarDerecha(Node *y){
    Node *x = y->izquierda;
    Node *T2 = x->derecha;

    x->derecha = y;
    y->izquierda = T2;

    y->height = calcAltura(y);
    x->height = calcAltura(x);

    return x; /// NUEVA RAIZ
}

Node *rotarIzquierda(Node *x){
    Node *y = x->derecha;
    Node *T2 = y->izquierda;

    y->izquierda = x;
    x->derecha = T2;

    x->height = calcAltura(x);
    y->height = calcAltura(y);

    return y; /// NUEVA RAIZ
}

int GetBalance(Node *nodo){
    /// OTRA FUNCION UTILITARIA PARA AHORRAR CODIGO
    if (nodo == NULL)
        return 0;
    return height(nodo->izquierda) - height(nodo->derecha);
}

Node* insertar(Node *nodo, char *data){

    //1. Insertar
    if (nodo == NULL)                                           /// SI ESTA VACIO
        return (newNode(data));
    if (strcmp(data, nodo->dato) < 0)                           /// ENCONTRAR LA POSICION CORRECTA
        nodo->izquierda = insertar(nodo->izquierda, data);
    else if (strcmp(data, nodo->dato) > 0)
        nodo->derecha = insertar(nodo->derecha, data);
    else                                                        /// SON IGUALES, ES DECIR, ESTA REPETIDO
        return nodo;

    //2. Actualizar la altura del nuevo nodo
    nodo->height = calcAltura(nodo);

    //3. Hacer el balance para ver si quedo degenerado (Se explican en un archivo aparte)
    int balance = GetBalance(nodo);
    if (balance > 1 && strcmp(data, nodo->izquierda->dato) < 0)     /// IZQUIERDA IZQUIERDA
        return rotarDerecha(nodo);
    if (balance < -1 && strcmp(data, nodo->derecha->dato) > 0)      /// DERECHA DERECHA
        return rotarIzquierda(nodo);
    if (balance > 1 && strcmp(data, nodo->izquierda->dato) > 0){    /// IZQUIERDA DERECHA
        nodo->izquierda = rotarIzquierda(nodo->izquierda);
        return rotarDerecha(nodo);
    }
    if (balance < -1 && strcmp(data, nodo->derecha->dato) < 0){     /// DERECHA IZQUIERDA
        nodo->derecha = rotarDerecha(nodo->derecha);
        return rotarIzquierda(nodo);
    }

    return nodo;                                                    /// SIN EFECTUAR ROTACIONES
}

Node* Carga(char* direccion){
    /** PARA SIMPLIFICAR LA CARGA,
        Se le pasa la direccion, abre el archivo
        Y va insertando las frases
    */
	FILE *file = fopen(direccion, "r");
	if (!file)
        return;

	Node* node = NULL;
	char buffer [128], ch;
    int i = 0;
	while((ch = fgetc(file)) != EOF){
        if (ch != '\n'){
            buffer[i] = ch;
            i++;
        }else{
            buffer[i] = '\0';
            i = 0;
            node = insertar(node, buffer);
        }
    }
    fclose(file);
    return node;
}

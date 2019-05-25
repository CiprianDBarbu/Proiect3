#include <iostream>
#include <stack>

using namespace std;
///////////////////////////////////////////////////////////////////////
/* NOD */

template<typename K,typename V> class Dictionar;

template<class K,class V>
class Nod
{
    K key;
    V value;
    Nod<K,V> *left,*right;
    int inaltime;

    friend class Dictionar<K,V> ;

public:
    Nod();
    Nod(K k, V v);
    void afisare();

};

//////////////////////////////////////////////////////////////////////////

template<class K,class V>
Nod<K,V>::Nod()
{
    left = NULL;
    right = NULL;
    inaltime = 1;
    key = 0;
    value = 0;
}

template<class K,class V>
Nod<K,V>::Nod(K k, V v)
{
    left = NULL;
    right = NULL;
    this->key = k;
    this->value = v;
    inaltime = 1;
}

template<class K,class V>
void Nod<K,V>::afisare()
{
    cout<<key<<"  "<<value<<'\n';
}
/////////////////////////////////////////////////////////////////////////////
/* DICTIONAR */



template<class K,class V>
class Dictionar
{
    Nod<K,V> *root;
    friend class Nod<K,V>;

public:
    Dictionar();
    Dictionar(const Dictionar&); //constructor de copiere

    int maxim(int, int);
    int get_inaltime(Nod<K,V> *);
    int echilibrare(Nod<K,V> *);    //gaseste factorul de echilibrare
    Nod<K,V> *rotD(Nod<K,V> *);     //rotatie dreapta
    Nod<K,V> *rotS(Nod<K,V> *);     //rotatie stanga

    Nod<K,V> *insereaza(Nod<K,V>* , K, V);    //inserare generala
    void insereaza_rad(K, V);               //inserare pe radacina
    Nod<K,V> *stergere_nod(Nod<K,V>*, K);
    void sterge_nod(K k); //sterge un nod de valoare k
    bool cauta_nod(K k);
    Nod<K,V> *cauta(Nod<K,V>*, K k);


    void stergere_tot(Nod<K,V>*);
    void sterge_tot();  //sterge tot dictionarul



    void afisare_1();   //afisare in ordinea introducerii
    void afisare_1_R(Nod<K,V>*);
    void afisare_2();   //afisare RSD
    void afisare_2_R(Nod<K,V>*);


    Dictionar<K,V> &operator =(Dictionar<K,V>);
    V operator [](K);



};


///////////////////////////////////////////////////////////////////////
template<class K,class V>
Dictionar<K,V>::Dictionar()
{
    root = NULL;
}

template<class K,class V>
Dictionar<K,V>::Dictionar(const Dictionar &n)
{
    root = NULL;
    stack <Nod <K,V>*> S;
    S.push(n.root);
    while(!S.empty())
    {
        Nod <K,V> *aux;
        aux = S.top();
        S.pop();

        if(aux->left != NULL)
            S.push(aux->left);
        if(aux->right != NULL)
            S.push(aux->right);

        insereaza_rad(aux->key,aux->value);
    }
}

template<class K,class V>
Dictionar<K,V>& Dictionar<K,V>::operator =(Dictionar<K,V> n)
{
    swap(root , n.root);

    return *this;
}

template<class K,class V>
V Dictionar<K,V>::operator [](K k)
{
    Nod<K,V> *aux;
    aux = cauta(root,k);

    return aux->value;
}

template<class K,class V>
int Dictionar<K,V>::maxim(int a, int b)
{
    if(a>b)
        return a;
    else
        return b;
}

template<class K,class V>
int Dictionar<K,V>::get_inaltime(Nod<K,V> *n)
{
    if(n == NULL)
        return 0;
    return n->inaltime;
}

template<class K,class V>
int Dictionar<K,V>::echilibrare(Nod<K,V> *n)
{
    if(n == NULL)
        return 0;
    return (get_inaltime(n->left) - get_inaltime(n->right));
}

template<class K,class V>
Nod<K,V>* Dictionar<K,V>::rotD(Nod<K,V> *n)
{
    Nod<K,V> *a = n->left;      //in a se retine stanga nodului primit
    Nod<K,V> *ra = a->right;    //in ra se retine dreapta lui a

    a->right = n;
    n->left = ra;

    n->inaltime = maxim(get_inaltime(n->left) , get_inaltime(n->right))+ 1;
    a->inaltime = maxim(get_inaltime(a->left) , get_inaltime(a->right))+ 1;
    return a;
}

template<class K,class V>
Nod<K,V>* Dictionar<K,V>::rotS(Nod<K,V> *n)
{
    Nod<K,V> *a = n->right;
    Nod<K,V> *la = n->left; //left a

    a->left = n;
    n->right = la;

    n->inaltime = maxim(get_inaltime(n->left) , get_inaltime(n->right))+ 1;
    a->inaltime = maxim(get_inaltime(a->left) , get_inaltime(a->right))+ 1;
    return a;
}

template<class K,class V>
Nod<K,V>* Dictionar<K,V>::insereaza(Nod<K,V> *nrad,K k,V v)
{
    if(nrad == NULL)
        {
            nrad = new Nod<K,V>(k,v);
            return nrad;
        }

    if(nrad->key == k)
    {
        nrad->value = v;
        return nrad;
    }

    if(k < nrad->key)
        nrad->left = insereaza(nrad->left, k, v);
    else if(k > nrad->key)
        nrad->right = insereaza(nrad->right, k, v);
        else
            return nrad;

    nrad->inaltime = 1 + maxim(get_inaltime(nrad->left) , get_inaltime(nrad->right));

    int fctblns; //factorul de balansare
    fctblns = echilibrare(nrad);
    if(fctblns>1 && k < nrad->left->key)    //dubla stanga
        return rotD(nrad);
    if(fctblns<-1 && k > nrad->right->key)  //dubla dreapta
        return rotS(nrad);
    if(fctblns>1 && k > nrad->left->key)    //rotatie dubla dreapta
    {
        nrad->left = rotS(nrad->left);
        return rotD(nrad);
    }
    if(fctblns<-1 && k < nrad->right->key)  //rotatei dubla stanga
    {
        nrad->right = rotD(nrad->right);
        return rotS(nrad);
    }
    return nrad; //nici o schimbare facuta
}

template<class K,class V>
void Dictionar<K,V>::insereaza_rad(K k,V v)
{
   this-> root = insereaza(this->root, k, v);
}

template<class K,class V>
Nod<K,V>* Dictionar<K,V>::stergere_nod(Nod<K,V>* n, K k)
{
    if(n == NULL)
        return n;

    if(k < n->key)
        n->left = stergere_nod(n->left,k); //merge stanga
    else
        if(k > n->key)
            n->right = stergere_nod(n->right,k); //dreapta
        else
        {
        Nod<K,V> *aux;
        if(n->left == NULL || n->right == NULL)
        {
            if(n->left != NULL)    // copil stang
                {
                    aux = n->left;
                    *n = *aux;
                    delete aux;
                }
            else if(n->right != NULL)  //copil drept
                {
                    aux = n->right;
                    *n = *aux;
                    delete aux;
                }
                else if(n->left == NULL && n->right == NULL)  //nici un copil
                {
                    aux = n;
                    n = NULL;
                }
            delete aux;
        }
        else  //ambii copii
        {
            aux = n->right;
            while(aux->left != NULL)
                aux = aux->left;
            n->key = aux->key;
            n->right = stergere_nod(n->right,aux->key);
        }

        }

        n->inaltime = maxim(get_inaltime(n->left) , get_inaltime(n->right))+ 1;

        int fctblns; //factorul de balansare
        fctblns = echilibrare(n);

        if(fctblns>1 && k < n->left->key)    //dubla stanga
        return rotD(n);
        if(fctblns<-1 && k > n->right->key)  //dubla dreapta
        return rotS(n);
        if(fctblns>1 && k > n->left->key)    //rotatie dubla dreapta
        {
            n->left = rotS(n->left);
            return rotD(n);
        }
        if(fctblns<-1 && k < n->right->key)  //rotatei dubla stanga
        {
            n->right = rotD(n->right);
            return rotS(n);
        }
       return n;

}

template<class K,class V>
void Dictionar<K,V>::sterge_nod(K k)
{
    stergere_nod(root,k);
}

template<class K,class V>
void Dictionar<K,V>::afisare_1()
{
    afisare_1_R(root);
}

template<class K,class V>
void Dictionar<K,V>::afisare_1_R(Nod<K,V> *n)
{
    if(n)
    {
        afisare_1_R(n->left);
        n->afisare();
        afisare_1_R(n->right);
    }
}

template<class K,class V>
void Dictionar<K,V>::afisare_2()
{
    afisare_2_R(root);
}

template<class K,class V>
void Dictionar<K,V>::afisare_2_R(Nod<K,V> *n)
{
    if(n)
    {
        n->afisare();
        afisare_2_R(n->left);
        afisare_2_R(n->right);
    }
}

template<class K,class V>
bool Dictionar<K,V>::cauta_nod(K k)
{
    if(cauta(root,k)!= NULL)return true;
    return false;
}

template<class K,class V>
Nod<K,V>* Dictionar<K,V>::cauta(Nod<K,V> *n,K k)
{
    if(n == NULL || n->key == k)
        return n;
    if(n->key < k)
        return cauta(n->right , k);
    return cauta(n->left , k);
}

template<class K,class V>
void Dictionar<K,V>::stergere_tot(Nod<K,V> *n)
{
    if(n == NULL)return;

    stergere_tot(n->left);
    stergere_nod(n->right);

    delete n;
}

template<class K,class V>
void Dictionar<K,V>::sterge_tot()
{
    stergere_tot(root);
    root = NULL;
}

template<class K,class V>
ostream &operator <<(ostream &out,Dictionar<K,V> *n)
{
    n.afisare_1();
    return out;
}

int main()
{

    Dictionar<int,int> wow;
    wow.insereaza_rad(1,2);
    wow.insereaza_rad(2,2);
    wow.insereaza_rad(3,7);

    cout<<"Afisare 1:\n";
    wow.afisare_1();

    cout<<"\nAfisare 2:\n";
    wow.afisare_2();


    cout<<wow[3];

    Dictionar<int,int> wow2;

    wow2 = wow;

    cout<<endl;
    wow2.afisare_1();


    cout<<'\n';
    return 0;
}

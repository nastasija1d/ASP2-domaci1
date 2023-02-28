#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct node {
	int key;
	int b;
	int level;
	struct node* left, * right;
}	node;

typedef struct queue {
	node** q;
	int f, r;
}	queue;

node* createNode(int k) {
	node* p = malloc(sizeof(node));
	p->key = k;
	p->b = 0;
	p->left = NULL;
	p->right = NULL;
	return p;
}
 
int** generate_matrix(int n,int m) {
	int i, j, a;
	int* p, ** pp;
	pp = malloc(n * sizeof(int*));
	printf("\nUnesite opseg u kom zelite da budu vrednosti matrice\n");
	scanf("%d", &a);
	p = malloc(m * sizeof(int));
	pp[0] = p;
	//srand(time(NULL));
	pp[0][0] = rand() % (a+1);
	for (j = 1; j < m; j++) {
		//srand(time(NULL));
		pp[0][j] = (rand()*50+13) % (pp[0][j - 1]+1);
	}
	for (i = 1; i < n; i++) {
		p = malloc(m * sizeof(int));
		*(pp + i) = p;
		//srand(time(NULL));
		pp[i][0] = rand() % (pp[i - 1][0]+1);
		for (j = 1; j < m; j++) {
			a = min(pp[i - 1][j], pp[i][j - 1]);
			//srand(time(NULL));
			pp[i][j] = (rand()+109) %( a+1);
		}
	}
	return pp;
}

int** read_Matrix(int n,int m) {
	int i, j, ** pp, * p, a;
	pp = malloc(n * sizeof(int*));
	printf("\nUnesite elemente matrice\n");
	for (i = 0; i < n; i++) {
		p = malloc(m * sizeof(int));
		*(pp + i) = p;
		for (j = 0; j < m; j++) {
			scanf("%d", &a);
			pp[i][j] = a;
		}
	}
	return pp;
}

void deleteMatrix(int** p, int  n) {
	int i;
	for (i = 0; i < n; i++) {
		free(p[i]);
	}
	free(p);
}

int searchMatrix(int** p, int n, int m,int k) {
	int i, j,a=0;
	i = 0;
	j = m - 1;
	while (1) {
		a++;
		if (k == p[i][j]) {
			printf("Kljuc %d pronadjen na lokaciji (%d,%d)\n", k, i, j);
			break;
		}
		if (k < p[i][j]) {
			if (i<n-1) {
				i++;
			}
			else {
				printf("Kljuc %d nije pronadjen\n",k);
				break;
			}
		}
		if (k > p[i][j]) {
			if (j > 0) {
				j--;
			}
			else {
				printf("Kljuc %d nije pronadjen\n",k);
				break;
			}
		}
	}
	return a;
}

void print_matrix(int** pp,int n, int m) {
	int i, j;
	for (i = 0; i < n; i++) {
		for (j = 0; j < m; j++) {
			printf("%3d", pp[i][j]);
		}
		printf("\n");
	}
}

void rightRotation(node* x) {
	node* t, * y;
	y = x->left;
	t = y->right;
	y->right = x;
	x->left = t;
	return;
}

void leftRotation(node* x) {
	node* t, * y;
	y = x->right;
	t = y->left;
	y->left = x;
	x->right = t;
	return;
}

node * AVLinsert(node* root, int k) {
		node* p, * q, * fp, * x, * fx, * y;
		int i;
//UMETANJE NOVOG CVORA
		if (root == NULL) {
			root = createNode(k);
			return root;
		}
		p = x = root;
		fp = fx = NULL;
		while (p != NULL) {
			if (p->key <= k) {
				q = p->right;
			}
			else {
				q = p->left;
			}
			if (q != NULL) {
				if (q->b != 0) {
					fx = p;
					x = q; //kritican cvor
				}
			}
			fp = p;
			p = q;
		}
		q = createNode(k);
		if (k >= fp->key) {
			fp->right = q;
		}
		else {
			fp->left = q;
		}
//AZURIRANJE BALANSA OD KRITICNOG CVORA X
		if (k < x->key) {
			p = x->left;
		}
		else {
			p = x->right;
		}
		y = p;
		while (p != q) {
			if (k < p->key) {
				p->b = 1;
				p = p->left;
			}
			else {
				p->b = -1;
				p = p->right;
			}
		}
//PROVERA DA LI JE BALANS NARUSEN
		i = -1;
		if (k < x->key) {
			i = 1;
		}
		if (x->b == 0) {	
			x->b = i;
			return root;
		}
		if (x->b != i) {
			x->b = 0;
			return root;
		}
//NAGINJANJE NA JEDNU STRANU
		if (y->b == i) {
			p = y;
			if (i == 1) {
				rightRotation(x);
			}
			else {
				leftRotation(x);
			}
			x->b = 0;
			y->b = 0;
		}
//NAGINJANJE NA RAZLICITE STRANE
		else {
			if (i == 1) {
				p = y->right;
				leftRotation(y);
				x->left = p;
				rightRotation(x);
			}
			else {
				p = y->left;
				x->right = p;
				rightRotation(y);
				leftRotation(x);
			}
			if (p->b == 0) {
				x->b = 0;
				y->b = 0;
			}
			else {
				if (p->b == i) {
					x->b = -i;
					y->b = 0;
				}
				else {
					x->b = 0;
					y->b = i;
				}
				p->b = 0;
			}
		}
//AZURIRANJE NOVOG KORENA PODSTABLA
		if (fx == NULL) {
			root = p;
		}
		else {
			if (x == fx->right) {
				fx->right = p;
			}
			else {
				fx->left = p;
			}
		}
	return root;
}

node* makeTree(int** pp, int n, int m) {
	node* root = NULL;
	int i, j;
	for (i = 0; i < n; i++) {
		for (j = 0; j < m; j++) {
			root=AVLinsert(root, pp[i][j]);
		}
	}
	return root;
}

queue* createQueue(int a) {
	queue* qu = malloc(sizeof(queue));
	qu->q = malloc(a * sizeof(node*));
	qu->f = 0;
	qu->r = 0;
	return qu;
}

int queueEmpty(queue* q) {
	if (q->f == q->r) {
		return 1;
	}
	return 0;
}

void insertQueue(queue* qu, node* n) {
	qu->q[qu->r] = n;
	qu->r++;
}

node* deleteQueue(queue* qu) {
	node* x = qu->q[qu->f];
	qu->f++;
	return x;
}

void printTree(node *root,int n, int m) {
	if (root == NULL) {
		printf("\nDrvo je prazno");
		return;
	}
	node* p;
	node* o = createNode(-1);
	int i,j,l;
	l = 6;
	queue* qu = createQueue(n * m*2);
	root->level = 0;
	i = 0;
	for (j = 0; j < pow(2, l) - 2; j++) printf(" ");
	printf("%4d", root->key);
	insertQueue(qu, root);

	while (!queueEmpty(qu)) {
		p = deleteQueue(qu);
		
		if (p->key == -1) {
			for (j = 0; j < pow(2, l + 2 - i); j++) {
				printf(" ");
			}
		}
		else {
			if (p->level >= i) {
				printf("\n\n\n");
				i++;
			}
			for (j = 0; j < pow(2, (l - i)) - 2; j++) printf(" ");
			if (p->left != NULL) {
				printf("%4d", p->left->key);
				p->left->level = p->level + 1;
				insertQueue(qu, p->left);
			}
			else {
				o->level = p->level + 1;
				insertQueue(qu, o);
				printf("   X");
			}
			for (j = 0; j < pow(2, (l + 1 - i)) - 4; j++) printf(" ");
			if (p->right != NULL) {
				printf("%4d", p->right->key);
				p->right->level = p->level + 1;
				insertQueue(qu, p->right);
			}
			else {
				o->level = p->level + 1;
				insertQueue(qu, o);
				printf("   X");
			}
			for (j = 0; j < pow(2, (l - i)) - 2; j++) printf(" ");
		}
	}
	free(qu->q);
	free(qu);
	return;	
}

void deleteTree(node* root,int n,int m) {
	node* p = root;
	queue* qu = createQueue(m * n);
	insertQueue(qu, p);
	while (!queueEmpty(qu)) {
		p = deleteQueue(qu);
		if (p->left != NULL) {
			insertQueue(qu, p->left);
			p->left = NULL;
		}
		if (p->right != NULL) {
			insertQueue(qu, p->right);
			p->right = NULL;
		}
		free(p);
	}
	free(qu->q);
	free(qu);
	return;
}

int BST_search(node * root,int k) {
	node* p = root;
	int i = 1;
	while (p != NULL) {
		if (k == p->key) {
			printf("Kljuc je pronadjen\n");
			return i;
		}
		if (k > p->key) {
			p = p->right;
			i++;
		}
		else {
			p = p->left;
			i++;
		}
	}
	printf("Kljuc nije pronadjen\n");
	return i;
}



void main() {
	int n, m, ** p,a,b,c,d,k,s,l,s1,s2;
	float f,f1,f2;
	node* root;
	printf("Dobro dosli, izaberite nacin unosa matrice:\n1.Putem konzole\n2.Generisi matricu\n");
	scanf("%d", &a);
	printf("\nUnesite dimenzije matrice:\n");
	scanf("%d%d", &n, &m);
	if (a == 1) {
		p = read_Matrix(n, m);
	}
	else {
		if (a == 2) {
			p = generate_matrix(n, m);
		}
		else {
			printf("\nNekorektan unos!");
			exit(0);
		}
	}
	root = makeTree(p, n, m);
	while (1) {
		printf("\nOdaberite jednu od narednih opcija:\n1.Ispisi matricu\n2.Pretrazi Matricu na zadati kljuc\n3.Pretrazi matricu na generisanu sekvencu kljuceva");
		printf("\n4.Ispisi formatirano stablo\n5.Pretrazi stablo na zadati kljuc\n6.Uporedi performanse stabla i matrice\n7.Zavrsi program\n\n");
		scanf("%d", &a);
		if (a == 1) {
			print_matrix(p, n, m);
			printf("\n");
		}
		if (a == 2) {
			printf("\nUnesite kljuc koji zelite da pronadjete:\n");
			scanf("%d", &b);
			searchMatrix(p, n, m, b);
		}
		if (a == 3) {
			printf("\nUnesite opseg vrednosti kljuceva za pretragu:\n");
			scanf("%d%d", &c, &d);
			s = 0;
			k = c + rand() % (d - c);
			s += searchMatrix(p, n, m, k);
			l = 1;
			f = s / l;
			printf("\nProsecan broj koraka je:%f", f);
			while (1) {
				printf("\nOdaberite jednu opciju:\n1.Generisanje novog kljuca\n2.Kraj\n");
				scanf("%d", &b);
				if (b == 1) {
					k = (c*d + rand() % (1+k))%(d-c);
					s += searchMatrix(p, n, m, k);
					l++;
					f = s / l;
					printf("\nProsecan broj koraka je:%f", f);
				}
				if (b == 2) {
					break;
				}
			}
		}
		if (a == 4) {
			printTree(root, n, m);
			printf("\n");
		}
		if (a == 5) {
			printf("\nUnesite kljuc koji zelite da pronadjete:\n");
			scanf("%d", &b);
			BST_search(root, b);
		}
		if (a == 6) {
			printf("\nUnesite opseg vrednosti kljuceva za pretragu:\n");
			scanf("%d%d", &c, &d);
			s1 = 0;
			s2 = 0;
			k = 5 * d - 3 * c;
			for (l = 0; l < 10 ; l++) {
				k = ((c+1) * d + rand() % (k+1)) % (d - c);
				s1 += searchMatrix(p, n, m, k);
				s2 += BST_search(root, k);
			}
			f1 = s1 / l;
			f2 = s2 / l;
			printf("\nProsecan broj koraka za nalazenje kljuca u matrici je %f, a u stablu je %f\n", f1, f2);
		}
		if (a == 7) {
			break;
		}
		
	}
	deleteTree(root, n, m);
	deleteMatrix(p, n);
	printf("\nProgram je zavrsen! Dovidjenja!");	
}
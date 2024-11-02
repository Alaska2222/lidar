#include <iostream>
#include <cmath>
#include <math.h>
#include <vector>
#include <chrono>
#include <random>
#define M_PI 3.14159265358979323846


using namespace std;

const int N = 50; //кількість партіклів (точність)

struct Line {
    double x1, y1, x2, y2;
};

struct Point {
    double x, y;
};


int n;
int m, k;
int state;

double SL, Sx, Sy;
double coord_x, coord_y;
double dx, dy;
double minx, miny, maxx, maxy;

vector<double> d; //вектор всіх променів від лідару (для кожн ітерації)
vector<double> particle_weight(N);
vector<double> prefix_sum(N);
vector<Point> particles(N);
vector<Line> walls;
vector<Point> particles_new(N);
vector<Point> vert;

unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine generator(seed);


Point next_point_calc(Point particle, int index) {
    Point p;
    p.x = particle.x + d[index] * cos(((double)2.0 * M_PI / k) * index);
    p.y = particle.y + d[index] * sin(((double)2.0 * M_PI / k) * index);
    return p;
}

double avr_dist(double distance) {
    distance /= k;
    return (double)sqrt(distance);
}

double normal_dist(double err) { //функція нормального розподілу
    return ((1.0 / (sqrt(2.0 * M_PI) * (SL))) * (exp(((-0.5 * err * err) / (SL * SL)))));
}

double normal_random(double upper) {
    normal_distribution<double> distribution(0.0, upper);
    return distribution(generator);
}

double uniform_random() {
    uniform_real_distribution<double> unif(0.0, 1.0);
    return unif(generator);
}

double dist(Line wall, Point p) { 

    double A = p.x - wall.x1;
    double B = p.y - wall.y1;
    double C = wall.x2 - wall.x1;
    double D = wall.y2 - wall.y1;

    //AB - координати вектора АЕ;
    //СD - координати вектора АВ;



    double dot = A * C + B * D; //векторний добуток 
    double len_sq = C * C + D * D; //довжина стінки(модуль ветора АВ)
    double param = -1;
    if (len_sq != 0)
        param = dot / len_sq;

    double xx, yy; //координати найблищої точки на стінці

    if (param < 0) {
        xx = wall.x1;
        yy = wall.y1;
    }
    else if (param > 1) {
        xx = wall.x2;
        yy = wall.y2;
    }
    else {
        xx = wall.x1 + param * C;
        yy = wall.y1 + param * D;
    }
    double dx = p.x - xx;
    double dy = p.y - yy;
    return sqrt(dx * dx + dy * dy);// відстань від шуканої точки до стінки
}

double min_dist(Point p) {
    double distance = 1e9;
    for (int i = 0; i < walls.size(); ++i) {
        distance = min(distance, dist(walls[i], p)); //шукаємо найменьшу відстань до нйблищої стінки
    }
    return distance;
}



void weight_calc() {
    double res;
    for (int i = 0; i < N; ++i) {
        res = 0;

        for (int j = 0; j < k; ++j) {
            Point next_point = next_point_calc(particles[i], j); //для кожного партікла шукаєм координати точки де закінчується лазер 
            double temp = min_dist(next_point); //рахуєм відстань від кінцевої точки лазера до найблищої стінки
            res += (temp * temp); //додаємо квадрат ціїє відстані
        }

        double avr_res = avr_dist(res); //середня відстань всіх лазерів для заданого партікла
        particle_weight[i] = normal_dist(avr_res); //вага це чим більше наш партікл підходить нам (чим більше вага тим краще)

    }
}

void repopulate() {
    double s = 0.0;
    for (int i = 0; i < N; ++i) s += particle_weight[i]; //сумуємо всі ваги всіх партіклів

    for (int i = 0; i < N; ++i) particle_weight[i] = particle_weight[i] / s; //ділим кожну вагу на суму всіх ваг 
                                                                             //робим їх в діапазоні від 0 до 1 (потрібно для рандому наступ точки)                                                    

    prefix_sum[0] = particle_weight[0];

    for (int i = 1; i < N; ++i) prefix_sum[i] = (prefix_sum[i - 1] + particle_weight[i]); //рахуєм префікс суми (кож наступ = сумі попередніх)
                                                                                          //сума всіх ваг буде рівна одиниці

    for (int i = 0; i < N; ++i) particles_new[i] = { particles[i].x, particles[i].y }; //новий вектор партіклів
    
    for (int i = 0; i < N; ++i) {
        double rand = uniform_random();
        std::vector<double>::iterator new_spawn = lower_bound(prefix_sum.begin(), prefix_sum.end(), rand); //пошук найменьшої найблищої границі
        particles[i] = particles_new[new_spawn - prefix_sum.begin()]; //перезаписуєм масив партіклів
    }
}

void sense() {
    weight_calc(); //підраховуємо вагу кожного партікла
    repopulate(); //частинки з малими вагами відкидаємо і перероджуєм в потрібному діапазоні
}

void move() {
    for (int i = 0; i < N; ++i) {
        particles[i].x += dx + normal_random(Sx);
        particles[i].y += dy + normal_random(Sy);
    }
}

int check(Point test) { //метод трасіровки
    int i, j, c = 0;
    for (i = 0, j = n - 1; i < n; j = i++) {
        if (((vert[i].y > test.y) != (vert[j].y > test.y)) && (test.x < (vert[j].x - vert[i].x) * (test.y - vert[i].y) / (vert[j].y - vert[i].y) + vert[i].x))
            c = !c;
    }
    return c;
}

double random(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

void generate_random_particles() {
    int i = 0;
    while (i < N) {
        double gen_x, gen_y;
        gen_x = random(minx, maxx);
        gen_y = random(miny, maxy);
        if (check({ gen_x, gen_y })) {
            particles[i] = { gen_x, gen_y };
            i++;
        }
    }
}



int main() {
    cin >> n;
    double a, b, fstx, fsty; cin >> a >> b;
    minx = a; maxx = a; miny = b; maxy = b; 
    fstx = a; fsty = b; //координати кінця останньої стінки
    walls.assign(n, { 0, 0 });
    vert.resize(n);
    vert[0] = { a, b };
    for (int i = 1; i < n; ++i) {
        double x_w, y_w; cin >> x_w >> y_w;
        Line wall = { a, b, x_w, y_w };
        walls[i] = wall;
        a = x_w;
        b = y_w;
        minx = min(minx, a);
        maxx = max(maxx, a);
        miny = min(miny, b);
        maxy = max(maxy, b);
        vert[i] = { a, b };
    }
    walls[0] = { a, b, fstx, fsty };

    cin >> m >> k;
    cin >> SL >> Sx >> Sy;
    cin >> state;

    if (state) {
        cin >> coord_x >> coord_y;
        for (int i = 0; i < N; ++i) particles[i] = { coord_x, coord_y };
    }
    else generate_random_particles();

    d.resize(k);
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < k; ++j) cin >> d[j];

        sense();
        cin >> dx >> dy;
        move();
    }

    for (int j = 0; j < k; ++j) cin >> d[j];

    sense();

    double res_x = 0, res_y = 0;

    for (int i = 0; i < N; ++i) {
        res_x += particles[i].x;
        res_y += particles[i].y;
    }

    res_x /= (double)N;
    res_y /= (double)N;

    cout << res_x << ' ' << res_y << endl;
}


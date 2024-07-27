Analisa Studi Kasus Tema Data Laporan
Studi Kasus:
Studi kasus ini berfokus pada pengembangan aplikasi pelaporan kerja harian yang berfungsi untuk mengelola laporan harian karyawan dalam sebuah perusahaan. Aplikasi ini harus mendukung operasi CRUD (Create, Read, Update, Delete) melalui REST API. Framework yang digunakan adalah Lumen dan database menggunakan Docker.

Kebutuhan:
1. Endpoint untuk pelaporan harian:
POST /reports: Untuk membuat laporan baru.
GET /reports: Untuk mendapatkan semua laporan.
GET /reports/{id}: Untuk mendapatkan laporan berdasarkan ID.
PUT /reports/{id}: Untuk memperbarui laporan berdasarkan ID.
DELETE /reports/{id}: Untuk menghapus laporan berdasarkan ID.

2. Autentikasi dan Autorisasi:
Menggunakan token JWT untuk mengamankan endpoint.
Struktur Data Laporan:
- ID
- Tanggal
- Deskripsi Pekerjaan
- Karyawan ID
- Waktu Mulai
- Waktu Selesai

Lingkungan Pengembangan:
- Framework: Lumen
- Database: MySQL yang dijalankan di dalam Docker container

Implementasi Source Code
1. Setup Lumen
Install Lumen:

sh
Copy code
composer create-project --prefer-dist laravel/lumen laporan-harian
cd laporan-harian
Konfigurasi .env:

env
Copy code
DB_CONNECTION=mysql
DB_HOST=mysql
DB_PORT=3306
DB_DATABASE=laporan_harian
DB_USERNAME=root
DB_PASSWORD=root

2. Setup Docker
Dockerfile:

dockerfile
Copy code
FROM php:7.4-fpm

RUN docker-php-ext-install pdo pdo_mysql

WORKDIR /var/www

COPY . .

RUN chown -R www-data:www-data /var/www

EXPOSE 9000
CMD ["php-fpm"]
docker-compose.yml:

yaml
Copy code
version: '3.1'

services:
  app:
    build:
      context: .
      dockerfile: Dockerfile
    container_name: lumen_app
    volumes:
      - .:/var/www
    ports:
      - "9000:9000"
    depends_on:
      - mysql

  mysql:
    image: mysql:5.7
    container_name: mysql
    environment:
      MYSQL_DATABASE: laporan_harian
      MYSQL_ROOT_PASSWORD: root
    ports:
      - "3306:3306"
    volumes:
      - mysql_data:/var/lib/mysql

volumes:
  mysql_data:
Jalankan Docker:

sh
Copy code
docker-compose up -d

3. Membuat Model dan Migration
Buat migration:

sh
Copy code
php artisan make:migration create_reports_table
Isi migration:

php
Copy code
use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Support\Facades\Schema;

class CreateReportsTable extends Migration
{
    public function up()
    {
        Schema::create('reports', function (Blueprint $table) {
            $table->id();
            $table->date('date');
            $table->text('description');
            $table->unsignedBigInteger('employee_id');
            $table->time('start_time');
            $table->time('end_time');
            $table->timestamps();
        });
    }

    public function down()
    {
        Schema::dropIfExists('reports');
    }
}
Jalankan migration:

sh
Copy code
php artisan migrate
Buat model:

php
Copy code
namespace App\Models;

use Illuminate\Database\Eloquent\Model;

class Report extends Model
{
    protected $fillable = [
        'date', 'description', 'employee_id', 'start_time', 'end_time'
    ];
}

4. Membuat Controller
Buat controller:

sh
Copy code
php artisan make:controller ReportController
Isi controller:

php
Copy code
namespace App\Http\Controllers;

use App\Models\Report;
use Illuminate\Http\Request;

class ReportController extends Controller
{
    public function index()
    {
        return response()->json(Report::all());
    }

    public function show($id)
    {
        return response()->json(Report::find($id));
    }

    public function store(Request $request)
    {
        $report = Report::create($request->all());
        return response()->json($report, 201);
    }

    public function update(Request $request, $id)
    {
        $report = Report::findOrFail($id);
        $report->update($request->all());
        return response()->json($report, 200);
    }

    public function destroy($id)
    {
        Report::destroy($id);
        return response()->json(null, 204);
    }
}

5. Definisikan Routes
Isi routes/web.php:
php
Copy code
$router->group(['prefix' => 'api'], function () use ($router) {
    $router->get('reports',  ['uses' => 'ReportController@index']);
    $router->get('reports/{id}', ['uses' => 'ReportController@show']);
    $router->post('reports', ['uses' => 'ReportController@store']);
    $router->put('reports/{id}', ['uses' => 'ReportController@update']);
    $router->delete('reports/{id}', ['uses' => 'ReportController@destroy']);
});

6. Menambahkan Autentikasi JWT
Install paket JWT:

sh
Copy code
composer require tymon/jwt-auth
Konfigurasi JWT di bootstrap/app.php:

php
Copy code
$app->register(Tymon\JWTAuth\Providers\LumenServiceProvider::class);

$app->routeMiddleware([
    'auth' => App\Http\Middleware\Authenticate::class,
]);
Publikasikan konfigurasi JWT:

sh
Copy code
php artisan vendor:publish --provider="Tymon\JWTAuth\Providers\LumenServiceProvider"
php artisan jwt:secret
Middleware Autentikasi:

php
Copy code
namespace App\Http\Middleware;

use Closure;
use Tymon\JWTAuth\Facades\JWTAuth;
use Exception;

class Authenticate
{
    public function handle($request, Closure $next, $guard = null)
    {
        try {
            $user = JWTAuth::parseToken()->authenticate();
        } catch (Exception $e) {
            return response()->json(['error' => 'Unauthorized'], 401);
        }

        return $next($request);
    }
}
Membuat endpoint login dan registrasi:

php
Copy code
// Di routes/web.php
$router->post('auth/login', ['uses' => 'AuthController@login']);
$router->post('auth/register', ['uses' => 'AuthController@register']);

// Di AuthController.php
namespace App\Http\Controllers;

use Illuminate\Http\Request;
use App\Models\User;
use Illuminate\Support\Facades\Hash;
use Tymon\JWTAuth\Facades\JWTAuth;

class AuthController extends Controller
{
    public function register(Request $request)
    {
        $user = User::create([
            'name' => $request->name,
            'email' => $request->email,
            'password' => Hash::make($request->password)
        ]);

        return response()->json($user, 201);
    }

    public function login(Request $request)
    {
        $credentials = $request->only('email', 'password');

        if (! $token = JWTAuth::attempt($credentials)) {
            return response()->json(['error' => 'Unauthorized'], 401);
        }

        return response()->json(compact('token'));
    }
}
Analisis Aplikasi Tema Data Laporan

Tujuan:
Aplikasi ini dirancang untuk memungkinkan pegawai melaporkan pekerjaan harian mereka, yang kemudian dapat dilihat dan dikelola oleh manajer atau administrator.

Fitur Utama:
1. Autentikasi Pengguna:
Registrasi dan login pengguna.
Pengguna dapat berupa pegawai atau manajer.

2. Pelaporan Kerja Harian:
Pegawai dapat menambahkan laporan kerja harian.
Setiap laporan berisi tanggal, deskripsi pekerjaan, dan durasi kerja.

3. Manajemen Laporan:
Manajer dapat melihat semua laporan yang telah dibuat oleh pegawai.
Fitur pencarian dan filter berdasarkan tanggal atau nama pegawai.

4. Dashboard:
Menampilkan ringkasan pekerjaan harian.
Statistik pekerjaan mingguan atau bulanan.

5. Role Management:
Pegawai hanya bisa melihat dan mengedit laporan mereka sendiri.
Manajer dapat mengelola laporan seluruh pegawai.

Langkah-langkah:
1. Instalasi Docker di Ubuntu
1.1 Instal Docker Engine
Pertama-tama, kita perlu menginstal Docker Engine di mesin Ubuntu Anda. Berikut adalah langkah-langkahnya:

bash
Copy code
sudo apt update
sudo apt install apt-transport-https ca-certificates curl software-properties-common -y
Tambahkan GPG key Docker resmi:

bash
Copy code
curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -
Tambahkan repository Docker ke sistem:

bash
Copy code
sudo add-apt-repository "deb [arch=amd64] https://download.docker.com/linux/ubuntu $(lsb_release -cs) stable"
Perbarui database paket dan instal Docker:

bash
Copy code
sudo apt update
sudo apt install docker-ce -y
Verifikasi instalasi Docker:

bash
Copy code
docker --version
Outputnya harus seperti ini:

Copy code
Docker version 20.10.8, build 3967b7d

1.2 Instal Docker Compose
Docker Compose digunakan untuk mengelola banyak kontainer. Berikut adalah langkah-langkahnya:

Download Docker Compose:

bash
Copy code
sudo curl -L "https://github.com/docker/compose/releases/download/v2.5.1/docker-compose-$(uname -s)-$(uname -m)" -o /usr/local/bin/docker-compose
Setel izin eksekusi:

bash
Copy code
sudo chmod +x /usr/local/bin/docker-compose
Verifikasi instalasi Docker Compose:

bash
Copy code
docker-compose --version
Outputnya akan seperti ini:

Copy code
docker-compose version 2.5.1, build a814d8a

2. Membuat Project Laravel Baru
2.1 Instal Laravel
Untuk membuat proyek Laravel baru, pastikan Anda memiliki Composer terinstal. Jika belum, instal Composer dengan perintah berikut:

bash
Copy code
sudo apt install curl php-cli php-mbstring unzip
curl -sS https://getcomposer.org/installer | php
sudo mv composer.phar /usr/local/bin/composer
Periksa instalasi Composer:

bash
Copy code
composer --version
Hasilnya akan menunjukkan versi Composer yang telah terinstal.

2.2 Membuat Proyek Laravel Baru
Sekarang, kita akan membuat proyek Laravel baru bernama laporan-kerja-harian:

bash
Copy code
composer create-project --prefer-dist laravel/laravel laporan-kerja-harian

2.3 Masuk ke Direktori Proyek
Masuk ke direktori proyek:

bash
Copy code
cd laporan-kerja-harian

3. Membuat Docker Setup
3.1 Membuat Dockerfile
Buat file Dockerfile di root direktori proyek Laravel:

dockerfile
Copy code
# Menggunakan PHP 8.1 FPM sebagai image dasar
FROM php:8.1-fpm

# Install sistem dependensi yang diperlukan dan PHP extensions
RUN apt-get update && apt-get install -y \
    git \
    curl \
    libpng-dev \
    libjpeg-dev \
    libfreetype6-dev \
    zip \
    unzip \
    && docker-php-ext-configure gd --with-freetype --with-jpeg \
    && docker-php-ext-install gd pdo pdo_mysql

# Install Composer
COPY --from=composer:2.5 /usr/bin/composer /usr/bin/composer

# Set working directory
WORKDIR /var/www/html

# Salin semua file ke direktori kerja
COPY . .

# Install dependensi Laravel
RUN composer install

# Berikan permission untuk storage dan bootstrap
RUN chmod -R 777 storage bootstrap/cache

# Expose port 8000 dan jalankan server Laravel
EXPOSE 8000
CMD php artisan serve --host=0.0.0.0 --port=8000

3.2 Membuat docker-compose.yml
Buat file docker-compose.yml di root direktori proyek Laravel:

yaml
Copy code
version: '3.8'

services:
  app:
    build:
      context: .
      dockerfile: Dockerfile
    image: laravel-app
    container_name: laporan_harian_app
    restart: unless-stopped
    ports:
      - "8000:8000"
    volumes:
      - .:/var/www/html
    depends_on:
      - db

  db:
    image: mysql:8.0
    container_name: laporan_harian_db
    restart: unless-stopped
    environment:
      MYSQL_ROOT_PASSWORD: password
      MYSQL_DATABASE: laporan_harian
      MYSQL_USER: laravel_user
      MYSQL_PASSWORD: password
    ports:
      - "3306:3306"
    volumes:
      - dbdata:/var/lib/mysql

volumes:
  dbdata:

3.3 Mengatur File .env
Buka file .env dan ubah konfigurasi database:

ini
Copy code
DB_CONNECTION=mysql
DB_HOST=db
DB_PORT=3306
DB_DATABASE=laporan_harian
DB_USERNAME=laravel_user
DB_PASSWORD=password

4. Menjalankan Aplikasi dengan Docker
4.1 Menjalankan Kontainer Docker
Jalankan perintah berikut untuk memulai kontainer Docker:

bash
Copy code
docker-compose up -d --build
Perintah ini akan melakukan hal berikut:

up: Menjalankan semua layanan yang didefinisikan dalam file docker-compose.yml.
-d: Menjalankan kontainer dalam mode detached (di latar belakang).
--build: Membangun ulang image jika ada perubahan.

4.2 Memeriksa Status Kontainer
Periksa apakah kontainer berjalan dengan benar:

bash
Copy code
docker-compose ps
Outputnya akan menampilkan layanan app dan db:

markdown
Copy code
   Name                 Command              State           Ports         
---------------------------------------------------------------------------
laporan_harian_app   docker-php-entrypoint php-fpm   Up      0.0.0.0:8000->8000/tcp
laporan_harian_db    docker-entrypoint.sh mysqld     Up      0.0.0.0:3306->3306/tcp

4.3 Mengakses Aplikasi
Sekarang aplikasi Laravel Anda seharusnya berjalan pada http://localhost:8000. Anda bisa membukanya di browser untuk memverifikasi bahwa instalasi berhasil.

4.4 Menjalankan Migrasi Database
Untuk menjalankan migrasi database, gunakan perintah berikut di dalam kontainer app:

bash
Copy code
docker-compose exec app php artisan migrate
Ini akan membuat tabel yang dibutuhkan dalam database laporan_harian.

5. Mengembangkan Aplikasi Laravel
Studi Kasus: Tema Data Laporan
Aplikasi ini akan memiliki dua jenis pengguna: pegawai dan manajer. Pegawai dapat membuat, mengedit, dan menghapus laporan kerja, sedangkan manajer dapat melihat semua laporan dari pegawai.

5.1 Membuat Model dan Migration
Jalankan perintah berikut untuk membuat model dan migrasi untuk laporan kerja:

bash
Copy code
docker-compose exec app php artisan make:model Report -m
Ini akan membuat model Report dan file migrasi untuk tabel reports.

5.2 Konfigurasi Migration untuk Laporan
Buka file migrasi yang baru saja dibuat di database/migrations dan tambahkan kolom yang diperlukan:

php
Copy code
use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Support\Facades\Schema;

class CreateReportsTable extends Migration
{
    /**
     * Run the migrations.
     *
     * @return void
     */
    public function up()
    {
        Schema::create('reports', function (Blueprint $table) {
            $table->id();
            $table->unsignedBigInteger('user_id');
            $table->date('date');
            $table->string('description');
            $table->integer('duration');
            $table->timestamps();

            $table->foreign('user_id')->references('id')->on('users')->onDelete('cascade');
        });
    }

    /**
     * Reverse the migrations.
     *
     * @return void
     */
    public function down()
    {
        Schema::dropIfExists('reports');
    }
}
Kolom yang ditambahkan meliputi:

user_id: Merujuk ke tabel users untuk menyimpan informasi pegawai yang membuat laporan.
date: Tanggal laporan dibuat.
description: Deskripsi pekerjaan yang dilakukan.
duration: Durasi kerja dalam jam.

5.3 Jalankan Migrasi
Jalankan migrasi untuk membuat tabel reports:

bash
Copy code
docker-compose exec app php artisan migrate

5.4 Membuat Controller dan Resource Route
Buat controller untuk mengelola laporan kerja:

bash
Copy code
docker-compose exec app php artisan make:controller ReportController --resource
Tambahkan resource route untuk ReportController di routes/web.php:

php
Copy code
use App\Http\Controllers\ReportController;

Route::resource('reports', ReportController::class)->middleware('auth');

5.5 Implementasi Controller
Buka app/Http/Controllers/ReportController.php dan tambahkan logika berikut:

php
Copy code
namespace App\Http\Controllers;

use App\Models\Report;
use Illuminate\Http\Request;
use Illuminate\Support\Facades\Auth;

class ReportController extends Controller
{
    public function index()
    {
        // Menampilkan semua laporan milik user yang sedang login
        $reports = Auth::user()->reports;
        return view('reports.index', compact('reports'));
    }

    public function create()
    {
        return view('reports.create');
    }

    public function store(Request $request)
    {
        // Validasi input user
        $request->validate([
            'date' => 'required|date',
            'description' => 'required|string|max:255',
            'duration' => 'required|integer|min:1',
        ]);

        // Membuat laporan baru
        Report::create([
            'user_id' => Auth::id(),
            'date' => $request->date,
            'description' => $request->description,
            'duration' => $request->duration,
        ]);

        return redirect()->route('reports.index')->with('success', 'Laporan berhasil ditambahkan.');
    }

    public function show(Report $report)
    {
        $this->authorize('view', $report);
        return view('reports.show', compact('report'));
    }

    public function edit(Report $report)
    {
        $this->authorize('update', $report);
        return view('reports.edit', compact('report'));
    }

    public function update(Request $request, Report $report)
    {
        $this->authorize('update', $report);

        // Validasi input user
        $request->validate([
            'date' => 'required|date',
            'description' => 'required|string|max:255',
            'duration' => 'required|integer|min:1',
        ]);

        // Update laporan
        $report->update($request->all());

        return redirect()->route('reports.index')->with('success', 'Laporan berhasil diupdate.');
    }

    public function destroy(Report $report)
    {
        $this->authorize('delete', $report);
        $report->delete();

        return redirect()->route('reports.index')->with('success', 'Laporan berhasil dihapus.');
    }
}

5.6 Membuat Views untuk Laporan
Buat folder reports di dalam direktori resources/views dan buat beberapa file view berikut:

index.blade.php

html
Copy code
@extends('layouts.app')

@section('content')
<div class="container">
    <h1>Daftar Laporan Kerja</h1>
    <a href="{{ route('reports.create') }}" class="btn btn-primary">Buat Laporan Baru</a>

    @if(session('success'))
        <div class="alert alert-success mt-3">
            {{ session('success') }}
        </div>
    @endif

    <table class="table mt-3">
        <thead>
            <tr>
                <th>Tanggal</th>
                <th>Deskripsi</th>
                <th>Durasi (Jam)</th>
                <th>Aksi</th>
            </tr>
        </thead>
        <tbody>
            @foreach($reports as $report)
                <tr>
                    <td>{{ $report->date }}</td>
                    <td>{{ $report->description }}</td>
                    <td>{{ $report->duration }}</td>
                    <td>
                        <a href="{{ route('reports.show', $report->id) }}" class="btn btn-info btn-sm">Lihat</a>
                        <a href="{{ route('reports.edit', $report->id) }}" class="btn btn-warning btn-sm">Edit</a>
                        <form action="{{ route('reports.destroy', $report->id) }}" method="POST" class="d-inline">
                            @csrf
                            @method('DELETE')
                            <button type="submit" class="btn btn-danger btn-sm" onclick="return confirm('Anda yakin ingin menghapus laporan ini?')">Hapus</button>
                        </form>
                    </td>
                </tr>
            @endforeach
        </tbody>
    </table>
</div>
@endsection
create.blade.php

html
Copy code
@extends('layouts.app')

@section('content')
<div class="container">
    <h1>Buat Laporan Baru</h1>

    <form action="{{ route('reports.store') }}" method="POST">
        @csrf
        <div class="form-group">
            <label for="date">Tanggal</label>
            <input type="date" class="form-control" id="date" name="date" required>
        </div>
        <div class="form-group">
            <label for="description">Deskripsi</label>
            <input type="text" class="form-control" id="description" name="description" required>
        </div>
        <div class="form-group">
            <label for="duration">Durasi (Jam)</label>
            <input type="number" class="form-control" id="duration" name="duration" required min="1">
        </div>
        <button type="submit" class="btn btn-primary">Simpan</button>
    </form>
</div>
@endsection
edit.blade.php

html
Copy code
@extends('layouts.app')

@section('content')
<div class="container">
    <h1>Edit Laporan</h1>

    <form action="{{ route('reports.update', $report->id) }}" method="POST">
        @csrf
        @method('PUT')
        <div class="form-group">
            <label for="date">Tanggal</label>
            <input type="date" class="form-control" id="date" name="date" value="{{ $report->date }}" required>
        </div>
        <div class="form-group">
            <label for="description">Deskripsi</label>
            <input type="text" class="form-control" id="description" name="description" value="{{ $report->description }}" required>
        </div>
        <div class="form-group">
            <label for="duration">Durasi (Jam)</label>
            <input type="number" class="form-control" id="duration" name="duration" value="{{ $report->duration }}" required min="1">
        </div>
        <button type="submit" class="btn btn-primary">Update</button>
    </form>
</div>
@endsection
show.blade.php

html
Copy code
@extends('layouts.app')

@section('content')
<div class="container">
    <h1>Detail Laporan Kerja</h1>

    <div class="card">
        <div class="card-header">
            <h3>{{ $report->date }}</h3>
        </div>
        <div class="card-body">
            <p><strong>Deskripsi:</strong> {{ $report->description }}</p>
            <p><strong>Durasi (Jam):</strong> {{ $report->duration }}</p>
            <a href="{{ route('reports.index') }}" class="btn btn-secondary">Kembali</a>
        </div>
    </div>
</div>
@endsection

5.7 Membuat Layout Dasar
Buat layout dasar di resources/views/layouts/app.blade.php:

html
Copy code
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>@yield('title', 'Aplikasi Laporan Kerja Harian')</title>
    <link href="{{ asset('css/app.css') }}" rel="stylesheet">
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0-alpha3/dist/css/bootstrap.min.css" rel="stylesheet">
</head>
<body>
    <nav class="navbar navbar-expand-lg navbar-light bg-light">
        <div class="container">
            <a class="navbar-brand" href="{{ url('/') }}">Laporan Harian</a>
            <button class="navbar-toggler" type="button" data-toggle="collapse" data-target="#navbarNav" aria-controls="navbarNav" aria-expanded="false" aria-label="Toggle navigation">
                <span class="navbar-toggler-icon"></span>
            </button>
            <div class="collapse navbar-collapse" id="navbarNav">
                <ul class="navbar-nav ml-auto">
                    @guest
                        <li class="nav-item">
                            <a class="nav-link" href="{{ route('login') }}">Login</a>
                        </li>
                        <li class="nav-item">
                            <a class="nav-link" href="{{ route('register') }}">Register</a>
                        </li>
                    @else
                        <li class="nav-item">
                            <a class="nav-link" href="{{ route('reports.index') }}">Laporan</a>
                        </li>
                        <li class="nav-item">
                            <a class="nav-link" href="{{ route('logout') }}" onclick="event.preventDefault(); document.getElementById('logout-form').submit();">Logout</a>
                            <form id="logout-form" action="{{ route('logout') }}" method="POST" style="display: none;">
                                @csrf
                            </form>
                        </li>
                    @endguest
                </ul>
            </div>
        </div>
    </nav>

    <main class="py-4">
        @yield('content')
    </main>

    <script src="{{ asset('js/app.js') }}"></script>
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0-alpha3/dist/js/bootstrap.bundle.min.js"></script>
</body>
</html>

5.8 Autentikasi Pengguna
Laravel sudah menyediakan sistem autentikasi yang dapat kita gunakan. Jalankan perintah berikut untuk membuat scaffolding autentikasi:

bash
Copy code
docker-compose exec app composer require laravel/ui:^3.2
docker-compose exec app php artisan ui bootstrap --auth
docker-compose exec app npm install
docker-compose exec app npm run dev
Jalankan migrasi untuk membuat tabel pengguna:

bash
Copy code
docker-compose exec app php artisan migrate
Sekarang, Anda sudah memiliki fitur login, registrasi, dan manajemen pengguna di aplikasi.

5.9 Mengatur Policy
Buat policy untuk memastikan hanya pemilik laporan yang dapat mengedit atau menghapusnya:

bash
Copy code
docker-compose exec app php artisan make:policy ReportPolicy --model=Report
Buka file app/Policies/ReportPolicy.php dan tambahkan logika berikut:

php
Copy code
namespace App\Policies;

use App\Models\Report;
use App\Models\User;
use Illuminate\Auth\Access\HandlesAuthorization;

class ReportPolicy
{
    use HandlesAuthorization;

    public function view(User $user, Report $report)
    {
        return $user->id === $report->user_id;
    }

    public function update(User $user, Report $report)
    {
        return $user->id === $report->user_id;
    }

    public function delete(User $user, Report $report)
    {
        return $user->id === $report->user_id;
    }
}
Tambahkan policy ini ke AuthServiceProvider:

php
Copy code
namespace App\Providers;

use App\Models\Report;
use App\Policies\ReportPolicy;
use Illuminate\Foundation\Support\Providers\AuthServiceProvider as ServiceProvider;

class AuthServiceProvider extends ServiceProvider
{
    protected $policies = [
        Report::class => ReportPolicy::class,
    ];

    public function boot()
    {
        $this->registerPolicies();
    }
}

6. Testing dan Debugging
Sekarang aplikasi sudah siap digunakan. Anda dapat melakukan testing dengan cara berikut:

6.1 Membuat Data Dummy
Gunakan factory dan seeder untuk membuat data dummy:

bash
Copy code
docker-compose exec app php artisan make:factory ReportFactory --model=Report
Isi file factory database/factories/ReportFactory.php:

php
Copy code
namespace Database\Factories;

use App\Models\Report;
use App\Models\User;
use Illuminate\Database\Eloquent\Factories\Factory;

class ReportFactory extends Factory
{
    protected $model = Report::class;

    public function definition()
    {
        return [
            'user_id' => User::factory(),
            'date' => $this->faker->date(),
            'description' => $this->faker->sentence(),
            'duration' => $this->faker->numberBetween(1, 8),
        ];
    }
}
Buat seeder untuk mengisi database dengan data dummy:

bash
Copy code
docker-compose exec app php artisan make:seeder ReportSeeder
Isi file seeder database/seeders/ReportSeeder.php:

php
Copy code
namespace Database\Seeders;

use App\Models\Report;
use Illuminate\Database\Seeder;

class ReportSeeder extends Seeder
{
    public function run()
    {
        Report::factory()->count(50)->create();
    }
}
Jalankan seeder:

bash
Copy code
docker-compose exec app php artisan db:seed --class=ReportSeeder

6.2 Testing dengan Browser
Buka http://localhost:8000 di browser dan login dengan pengguna yang telah Anda buat. Coba tambahkan, edit, dan hapus laporan untuk memastikan semuanya berjalan dengan baik.
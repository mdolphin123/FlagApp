package com.example.flagapp
import android.annotation.SuppressLint
import android.content.Intent
import android.os.Bundle
import android.os.Handler
import android.view.Menu
import android.view.View
import androidx.appcompat.app.AppCompatActivity
import androidx.core.content.ContextCompat
import androidx.navigation.findNavController
import androidx.navigation.ui.AppBarConfiguration
import androidx.navigation.ui.navigateUp
import com.androidnetworking.AndroidNetworking
import com.example.flagapp.databinding.ActivityMainBinding


@Suppress("DEPRECATION")

class MainActivity : AppCompatActivity() {
    private lateinit var appBarConfiguration: AppBarConfiguration
    private lateinit var binding: ActivityMainBinding


    @SuppressLint("UseCompatLoadingForDrawables")
    override fun onCreate(savedInstanceState: Bundle?) {
        AndroidNetworking.initialize(getApplicationContext());
        super.onCreate(savedInstanceState)
        setContentView(R.layout.app_bar_main)
        window.statusBarColor = ContextCompat.getColor(this, R.color.black)

        val button = findViewById<View>(R.id.button2)
        button.setOnClickListener {
            val handler = Handler()
            button.setBackgroundDrawable(getResources().getDrawable(R.drawable.roundcornerborder3))
            handler.postDelayed({
                val i = Intent(this@MainActivity, sign_in::class.java)
                startActivity(i)
            }, 500)
        }

        val button2 = findViewById<View>(R.id.button1)
        button2.setOnClickListener {
            val handler2 = Handler()
            button2.setBackgroundDrawable(getResources().getDrawable(R.drawable.roundcornerborder1))
            handler2.postDelayed({
                val i = Intent(this@MainActivity, register::class.java)
                startActivity(i)
            }, 500)
        }




            /*
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        //setSupportActionBar(binding.appBarMain.toolbar)

        binding.appBarMain.fab.setOnClickListener { view ->
            Snackbar.make(view, "Replace with your own action", Snackbar.LENGTH_LONG)
                .setAction("Action", null)
                .setAnchorView(R.id.fab).show()
        }

        val drawerLayout: DrawerLayout = binding.drawerLayout
        val navView: NavigationView = binding.navView
        val navController = findNavController(R.id.nav_host_fragment_content_main)
        // Passing each menu ID as a set of Ids because each
        // menu should be considered as top level destinations.
        appBarConfiguration = AppBarConfiguration(
            setOf(
                R.id.nav_home, R.id.nav_gallery, R.id.nav_slideshow
            ), drawerLayout
        )
        setupActionBarWithNavController(navController, appBarConfiguration)
        navView.setupWithNavController(navController)
        */
        }

        override fun onCreateOptionsMenu(menu: Menu): Boolean {
            // Inflate the menu; this adds items to the action bar if it is present.
            menuInflater.inflate(R.menu.main, menu)
            return true
        }

        override fun onSupportNavigateUp(): Boolean {
            val navController = findNavController(R.id.nav_host_fragment_content_main)
            return navController.navigateUp(appBarConfiguration) || super.onSupportNavigateUp()
        }
    }

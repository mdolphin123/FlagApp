package com.example.flagapp
import android.annotation.SuppressLint
import android.content.Intent
import android.os.Bundle
import android.os.Handler
import android.util.Log
import android.view.Menu
import android.view.View
import androidx.appcompat.app.AppCompatActivity
import androidx.core.content.ContextCompat
import androidx.navigation.findNavController
import androidx.navigation.ui.AppBarConfiguration
import androidx.navigation.ui.navigateUp
import com.amazonaws.mobile.client.internal.oauth2.OAuth2Client.TAG
import com.androidnetworking.AndroidNetworking
import com.example.flagapp.databinding.ActivityMainBinding
import androidx.lifecycle.Observer


@Suppress("DEPRECATION")

class MainActivity : AppCompatActivity() {
    private lateinit var appBarConfiguration: AppBarConfiguration
    private lateinit var binding: ActivityMainBinding


    @SuppressLint("UseCompatLoadingForDrawables")
    override fun onCreate(savedInstanceState: Bundle?) {
        AndroidNetworking.initialize(getApplicationContext());
        super.onCreate(savedInstanceState)
        val i = Intent(this@MainActivity, mainpage::class.java)
        startActivity(i)

        setContentView(R.layout.app_bar_main)
        window.statusBarColor = ContextCompat.getColor(this, R.color.black)


        val button = findViewById<View>(R.id.button1)
        // register a click listener
        /*
        button.setOnClickListener { view ->
            if (UserData.isSignedIn.value!!) {
                Backend.signOut()
            }
            else {
                Backend.signIn(this)
            }
        }

         */

        /* UserData.isSignedIn.observe(this, Observer<Boolean> { isSignedUp ->
            // update UI
            Log.i(TAG, "isSignedIn changed : $isSignedUp")
            if (isSignedUp) {
                val i = Intent(this@MainActivity, sign_in::class.java)
                startActivity(i)
            }
            else {
                val i = Intent(this@MainActivity, register::class.java)
                startActivity(i)
            }
        })

         */

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

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)
        Backend.handleWebUISignInResponse(requestCode, resultCode, data)
    }
}




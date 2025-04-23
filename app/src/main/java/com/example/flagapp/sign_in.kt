package com.example.flagapp

import android.content.Intent
import android.graphics.Color
import android.os.Bundle
import android.os.Handler
import android.view.View
import android.widget.Button
import android.widget.EditText
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import androidx.navigation.ui.AppBarConfiguration
import androidx.lifecycle.Observer
import android.util.Log
import com.amazonaws.mobile.client.internal.oauth2.OAuth2Client.TAG
import com.google.android.material.floatingactionbutton.FloatingActionButton

//import com.example.flagapp.databinding.ActivitySignInBinding

@Suppress("DEPRECATION")
class sign_in : AppCompatActivity() {

    private lateinit var appBarConfiguration: AppBarConfiguration
    private lateinit var binding: sign_in

    override fun onCreate(savedInstanceState: Bundle?) {

        super.onCreate(savedInstanceState)

        setContentView(R.layout.sign_in)



        val extras = intent.extras
        var username = ""
        var password = ""
        val usernameInput = findViewById<View>(R.id.editText2) as EditText
        val passwordInput = findViewById<View>(R.id.editText1) as EditText
        if(extras != null) {
            username = extras.getString("username").toString()
            password = extras.getString("password").toString()
        }

        val button4 = findViewById<View>(R.id.button) as Button

        button4.setOnClickListener {
            val handler3 = Handler()

            button4.setBackgroundDrawable(getResources().getDrawable(R.drawable.roundcornerborder1))
            handler3.postDelayed({
                val usernameenter = usernameInput.text.toString()
                val passwordenter = passwordInput.text.toString()
                if(!usernameenter.equals(username) || !passwordenter.equals(password)) {
                    button4.setBackgroundColor(Color.RED)
                    button4.setText("Error!")
                    val handler4 = Handler()
                    handler4.postDelayed({
                        val i = Intent(this@sign_in, sign_in::class.java)
                        i.putExtra("username", username);
                        i.putExtra("password", password)
                        startActivity(i)
                    }, 500)
                }
                else {
                    val i = Intent(this@sign_in, mainpage::class.java)
                    startActivity(i)
                }
            }, 500)
        }


    }
}
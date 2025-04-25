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

class register : AppCompatActivity() {
    private lateinit var appBarConfiguration: AppBarConfiguration
    private lateinit var binding: register

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.register)
        val usernameInput = findViewById<View>(R.id.editText2) as EditText
        val passwordInput = findViewById<View>(R.id.editText1) as EditText


        val button3 = findViewById<View>(R.id.button_reg) as Button
        button3.setOnClickListener {
            val handler3 = Handler()
            button3.setBackgroundDrawable(getResources().getDrawable(R.drawable.roundcornerborder1))
            handler3.postDelayed({
                val username = usernameInput.text.toString()
                val password = passwordInput.text.toString()

                if (username.length < 4) {
                    button3.setBackgroundColor(Color.RED)
                    button3.setText("Enter valid username!")
                    val handler4 = Handler()
                    handler4.postDelayed({
                        val i = Intent(this@register, register::class.java)
                        startActivity(i)
                    }, 500)
                } else if (password.length < 4) {
                    button3.setBackgroundColor(Color.RED)
                    button3.setText("Enter valid password!")
                    val handler4 = Handler()
                    handler4.postDelayed({
                        val i = Intent(this@register, register::class.java)
                        startActivity(i)
                    }, 500)
                } else {
                    val handler4 = Handler()
                    handler4.postDelayed({
                        val i = Intent(this@register, sign_in::class.java)
                        i.putExtra("username", username);
                        i.putExtra("password", password);
                        startActivity(i)
                    }, 500)
                }
            }, 500)
        }
    }
}
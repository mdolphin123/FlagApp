package com.example.flagapp

import android.content.Context
import android.os.Handler
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.CheckBox
import android.widget.CompoundButton
import android.widget.ImageView
import android.widget.TextView
import android.widget.Toast
import androidx.recyclerview.widget.RecyclerView
import com.androidnetworking.AndroidNetworking
import com.androidnetworking.error.ANError
import com.androidnetworking.interfaces.StringRequestListener


class CustomAdapter(private val mList: List<ItemsViewModel>, private val IPs: List<String>, private val context: Context) : RecyclerView.Adapter<CustomAdapter.ViewHolder>() {

    // create new views
    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ViewHolder {

        // inflates the card_view_design view
        // that is used to hold list item
        val view = LayoutInflater.from(parent.context)
            .inflate(R.layout.card_view_design, parent, false)

        return ViewHolder(view)
    }

    // binds the list items to a view
    override fun onBindViewHolder(holder: ViewHolder, position: Int) {
        val applicationContext = context
        val ItemsViewModel = mList[position]
        // sets the image to the imageview from our itemHolder class
        //holder.imageView.setImageResource(ItemsViewModel.image)

        // sets the text to the textview from our itemHolder class
        holder.checkbox.text = ItemsViewModel.text
        holder.checkbox.setChecked(ItemsViewModel.checked)
        //holder.textView.setId(ItemsViewModel.ID)
        holder.textView.text = ItemsViewModel.text2
        holder.checkbox.setOnClickListener {
            if(holder.checkbox.isChecked()) {
                //Also a placeholder, make sure you insert the FULL function!!!!
                val handler3 = Handler()

                AndroidNetworking.get("172.28.223.231/get?data=raise&" + "flag=" + IPs[position])
                    .build()
                    .getAsString(object : StringRequestListener {
                        override fun onResponse(response: String) {
                            val number: Int = response.toInt();
                            Toast.makeText(
                                applicationContext,
                                "Done: " + response.replace("\n", "") + "!",
                                Toast.LENGTH_LONG
                            ).show()
                        }

                        override fun onError(anError: ANError) {
                            Toast.makeText(
                                applicationContext,
                                anError.errorBody,
                                Toast.LENGTH_SHORT
                            ).show()
                        }
                    })

                handler3.postDelayed({
                    updateItem(mList, position, "full mast");
                    updateItem2(mList, position, true);
                }, 1000);



            }
            else {
                val handler3 = Handler()
                AndroidNetworking.get(IPs[position] + "/get?data=DOWN");
                handler3.postDelayed({
                    updateItem(mList, position, "ground");
                    updateItem2(mList, position, false);
                },1000)
            }
        }


    }

    // return the number of the items in the list
    override fun getItemCount(): Int {
        return mList.size
    }


    // Holds the views for adding it to image and text
    class ViewHolder(ItemView: View) : RecyclerView.ViewHolder(ItemView) {
        val checkbox: CheckBox = itemView.findViewById(R.id.checkbox)
        val textView: TextView = itemView.findViewById(R.id.textView8)
    }

    fun updateItem(list: List<ItemsViewModel>, position: Int, input: String) {
        list[position].text2 = input
        notifyItemChanged(position)
    }



    fun returnID(list: ArrayList<ItemsViewModel>, position: Int): Int {
        return list[position].ID;
    }

    fun ischecked(list: ArrayList<ItemsViewModel>, position: Int): Boolean {
        if(list[position].checked) {
            return true;
        }
        else {
            return false;
        }
    }
    fun updateItem2(list: List<ItemsViewModel>, position: Int, checking: Boolean) {
        list[position].checked = checking
    }
}
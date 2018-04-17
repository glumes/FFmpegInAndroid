package com.glumes.base.binder

import android.content.Intent
import android.databinding.DataBindingUtil
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import com.glumes.databindingadapter.BindingViewHolder
import com.glumes.databindingadapter.ViewHolderBinder
import com.glumes.ffmpeginandroid.R
import com.glumes.ffmpeginandroid.databinding.AdapterJumpActivityBinding

/**
 * Created by glumes on 17/04/2018
 */

data class JumpActivityItem(var title: String, var activityclass: Class<*>? = null)

class JumpActivityHandler {

    fun onClick(v: View, model: JumpActivityItem) {
        if (model.activityclass != null) {
            startActivity(v, model.activityclass!!)
        }
    }

    private fun startActivity(v: View, clazz: Class<*>) {
        v.context.startActivity(Intent(v.context, clazz))
    }
}

class JumpToActivityHolder(binding: AdapterJumpActivityBinding) : BindingViewHolder<JumpActivityItem, AdapterJumpActivityBinding>(binding) {

    override fun onBind(data: JumpActivityItem?, position: Int) {
        mBinding.model = data
        mBinding.handler = JumpActivityHandler()
        mBinding.executePendingBindings()
    }
}


class JumpActivityBinder : ViewHolderBinder<JumpActivityItem, JumpToActivityHolder>() {

    override fun onBindViewHolder(holder: JumpToActivityHolder?, data: JumpActivityItem?, position: Int) {
        holder!!.onBind(data, position)
    }

    override fun createViewHolder(inflater: LayoutInflater?, parent: ViewGroup?): JumpToActivityHolder {
        val mBinding = DataBindingUtil.inflate<AdapterJumpActivityBinding>(inflater!!, R.layout.adapter_jump_activity, parent, false)
        return JumpToActivityHolder(mBinding)
    }
}
